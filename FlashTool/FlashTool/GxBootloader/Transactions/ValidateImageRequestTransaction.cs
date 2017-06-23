using IntegratedDebugProtocol;
using IntegratedDebugProtocol.Transactions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gx.Transactions
{
    class ValidateImageRequestTransaction : Transaction
    {

        public override ushort Id => 0xB008;

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
