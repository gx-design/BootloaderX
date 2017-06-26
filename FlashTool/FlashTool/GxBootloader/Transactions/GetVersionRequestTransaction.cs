using IntegratedDebugProtocol.Transactions;
using System;
using System.Collections.Generic;
using System.Text;
using IntegratedDebugProtocol;

namespace Gx.Transactions
{
    public class GetVersionRequestTransaction : Transaction
    {
        public override ushort Id => 0xA002;

        public override bool Decode(List<byte> dataPayload)
        {
            return true;
        }

        public override bool Encode(IdpPacket packet)
        {
            return true;
        }
    }
}
