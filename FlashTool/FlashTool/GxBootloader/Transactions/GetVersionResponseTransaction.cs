using IntegratedDebugProtocol.Transactions;
using System;
using System.Collections.Generic;
using System.Text;
using IntegratedDebugProtocol;

namespace Gx.Transactions
{
    public class GetVersionResponseTransaction : Transaction
    {
        public float Version { get; set; }
        public float BootloaderVersion { get; set; }

        public string BuildString { get; set; }

        public override ushort Id => 0xA003;

        public override bool Decode(List<byte> dataPayload)
        {
            Version = dataPayload.ReadAt<float>(0);
            BootloaderVersion = dataPayload.ReadAt<float>(4);

            return true;
        }

        public override bool Encode(IdpPacket packet)
        {
            packet.Write(Version);

            return true;
        }
    }
}
