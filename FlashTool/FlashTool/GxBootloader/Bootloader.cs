using System;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using IdpProtocol;

namespace GX
{
    [Guid("182D7497-F3EE-4975-AD4D-F45686EBCB6E")]
    public class BootloaderXServiceNode : ClientNode
    {
        public async Task StartBootloader()
        {
            var(success, response) =
                await SendRequestAsync(OutgoingTransaction.Create(0xb501, CreateTransactionId(), IdpCommandFlags.None));
        }
    }

    [Guid("EBBF4FFB-0F3C-4975-83D2-B6FE4643D641")]
    public class GxBootloaderNode : ClientNode
    {
        public async Task<float> GetVersion()
        {
            float result = -1;

            var (success, response) = await SendRequestAsync(OutgoingTransaction.Create((UInt16)0xB001, CreateTransactionId()));

            if (success && response != null && response.ResponseCode == IdpResponseCode.OK)
            {
                result = response.Transaction.Read<float>();
            }

            return result;
        }

        public async Task<bool> EraseFlash() 
        {
            bool result = false;

            var (success, response) = await SendRequestAsync(OutgoingTransaction.Create((UInt16)0xB002, CreateTransactionId()), 10000);

            if( success && response != null && response.ResponseCode == IdpResponseCode.OK) 
            {
                result = true;
            }

            return result;
        }


        public async Task<bool> WriteBlock(byte[] data)
        {
            bool result = false;

            var (success, response) = await SendRequestAsync(
                OutgoingTransaction.Create((UInt16)0xB003, CreateTransactionId())
                .Write((byte)data.Length)
                .Write(data));

            if( success && response != null && response.ResponseCode == IdpResponseCode.OK) 
            {
                Console.WriteLine("Block written response ok");
                result = true;
            }
            else
            {
                Console.WriteLine("Block written response bad");
            }

            return result;
        }

        public async Task Commit()
        {
            bool result = false;

            var (success, response) = await SendRequestAsync(
                OutgoingTransaction.Create((UInt16)0xB004, CreateTransactionId()));

            if( success && response != null && response.ResponseCode == IdpResponseCode.OK) 
            {
                result = true;
            }
        }
    }
}
