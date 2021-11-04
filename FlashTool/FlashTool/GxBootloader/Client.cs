using System;
using System.IO;
using System.Net.Sockets;
using System.Reactive.Concurrency;
using System.Threading.Tasks;
using GX;
using IdpProtocol;

public class Client : NetworkQueryNode, IDisposable
{
    private GxBootloaderNode _bootloaderClientNode;
    private TcpClient _client;
    private Stream _dataStream;
    private IdpRouter _router;
    private StreamAdaptor _adaptor;

    private bool _isConnected;

    public Client() : base(Guid.Parse("5B210D51-EC44-4739-8DDE-850556A1AF9D"), "FlashTool")
    {
    }

    public bool Connect(IScheduler scheduler, string ip, int port)
    {
        if (!_isConnected)
        {
            try
            {
                _router = new IdpRouter();
                _router.AddNode(this);

                _client = new TcpClient();
                _client.Connect(ip, port);

                _dataStream = _client.GetStream();

                _adaptor = new StreamAdaptor(scheduler, _dataStream, _dataStream);

                _adaptor.ConnectionError += (sender, e) =>
                {
                    Dispose();
                };

                _router.AddAdaptor(_adaptor);
                _adaptor.Start();

                _isConnected = true;

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        return false;
    }

    public async Task ConnectInterfaces()
    {
        Console.WriteLine("connecting interfaces");
        var serviceNode = await QueryInterfaceAsync<BootloaderXServiceNode>();

        if (serviceNode != null)
        {
            Console.WriteLine("service node found");
            _router.AddNode(serviceNode);

            await serviceNode.WaitForEnumeration();

            ServiceNode = serviceNode;
        }
        else
        {
            Console.WriteLine("didnt find service node.");
            var bootloaderNode = await QueryInterfaceAsync<GxBootloaderNode>();

            
            if (bootloaderNode != null)
            {
                Console.WriteLine("bootloader node found");
                _router.AddNode(bootloaderNode);
                BootloaderClientNode = bootloaderNode;

                await bootloaderNode.WaitForEnumeration();
            }

            BootloaderClientNode = bootloaderNode;
        }
    }

    public async Task ConnectBootloaderInterfaces()
    {
        Console.WriteLine("querying for bootloader node.");
        var bootloaderNode = await QueryInterfaceAsync<GxBootloaderNode>();

        if (bootloaderNode != null)
        {
            Console.WriteLine("bootloader found.");
            _router.AddNode(bootloaderNode);
            BootloaderClientNode = bootloaderNode;

            await bootloaderNode.WaitForEnumeration();

            ServiceNode = null;
        }
        else
        {
            BootloaderClientNode = null;
        }
    }

    public GxBootloaderNode BootloaderClientNode { get; private set; }
    
    public BootloaderXServiceNode ServiceNode { get; private set; }


    public void Dispose()
    {
        _client.Dispose();
    }
}