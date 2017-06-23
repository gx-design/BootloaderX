namespace Gx.Transactions
{
    using IntegratedDebugProtocol;
    using IntegratedDebugProtocol.Transactions;
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;

    class ValidateImageResponseTransaction : Transaction
    {
        public bool Success { get; set; }

        public override ushort Id => 0xB009;

        public override bool Decode(List<byte> dataPayload)
        {
            Success = dataPayload.ReadAt<bool>(0);
            return true;
        }

        public override bool Encode(IdpPacket packet)
        {
            packet.Write(Success);
            return true;
        }
    }
}
