using IntegratedDebugProtocol;
using IntegratedDebugProtocol.Transactions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gx.Transactions
{
    class FlashDataResponseTransaction : Transaction
    {
        public bool Success { get; set; }

        public override ushort Id => 0xB007;

        public override bool Decode(List<byte> dataPayload)
        {
            Success = Convert.ToBoolean(dataPayload.ReadAt<byte>(0));
            return true;
        }

        public override bool Encode(IdpPacket packet)
        {
            packet.Write(Success);
            return true;
        }
    }
}
