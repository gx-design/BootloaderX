namespace IntegratedDebugProtocol.Transactions
{
    using System;
    using System.Collections.Generic;

    public abstract class Transaction
    {
        public abstract bool Encode(IdpPacket packet);

        public abstract bool Decode(List<byte> dataPayload);

        public abstract UInt16 Id { get; }
    }
}
