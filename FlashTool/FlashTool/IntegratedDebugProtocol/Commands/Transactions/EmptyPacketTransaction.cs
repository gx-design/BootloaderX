namespace IntegratedDebugProtocol.Commands.Transactions
{
    using IntegratedDebugProtocol.Transactions;
    using System.Collections.Generic;

    public abstract class EmptyPacketTransaction : Transaction
    {
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
