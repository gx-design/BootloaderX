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
        var bootloaderNode = await QueryInterfaceAsync<GxBootloaderNode>();

        if(bootloaderNode != null) 
        {
            _router.AddNode(bootloaderNode);
            BootloaderClientNode = bootloaderNode;

            await bootloaderNode.WaitForEnumeration();
        }
    }

    public GxBootloaderNode BootloaderClientNode { get; private set; }


    public void Dispose()
    {
        _client.Dispose();
    }
}