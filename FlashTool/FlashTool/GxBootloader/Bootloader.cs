using System;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using IdpProtocol;

namespace GX
{
    [Guid("33E95805-EA11-4B67-B79B-2B6227DC3BAF")]
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
                .Write((UInt16)data.Length)
                .Write(data));

            if( success && response != null && response.ResponseCode == IdpResponseCode.OK) 
            {
                result = true;
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
