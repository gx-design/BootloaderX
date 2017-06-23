using IntegratedDebugProtocol.Transactions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IntegratedDebugProtocol;

namespace Gx.Transactions
{
    class FlashDataRequestTransaction : Transaction
    {
        public byte Length { get; set; }
        public List<byte> Data { get; set; }

        public override ushort Id
        {
            get
            {
                return 0xB006;
            }
        }

        public override bool Decode(List<byte> dataPayload)
        {
            Length = dataPayload.ReadAt<byte>(0);
            Data = new List<byte>();

            for(int i = 0; i < Length; i++)
            {
                Data.Add(dataPayload.ReadAt<byte>(i + 1));
            }

            return true;
        }

        public override bool Encode(IdpPacket packet)
        {
            packet.Write(Length);

            foreach(var d in Data)
            {
                packet.Write(d);
            }

            return true;
        }
    }
}
