using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace IntegratedDebugProtocol
{
    class CRC16
    {
        public static ushort Crc16 (ushort oldCrc, byte data)
        {
            ushort crc;
            ushort x;

            x = (ushort)(((oldCrc >> 8) ^ data) & 0xff);
            x ^= (ushort)(x >> 4);

            crc = (ushort)((oldCrc << 8) ^ (x << 12) ^ (x << 5) ^ x);

            return crc;
        }
    }

    public class IdpPacket : List<byte>
    {
        private int _readIndex;

        public T Read<T>() where T : struct
        {
            var result = this.ReadAt<T>(_readIndex);         

            _readIndex += Marshal.SizeOf(typeof(T));

            return result;
        }

        /// <summary>
        /// Resets the data in the packet.
        /// </summary>
        /// <param name="sourceLink">Source link layer type.</param>
        /// <param name="destinationLink">Destination link layer type.</param>
        /// <param name="autoLength">Writes the bytes representing the length of the packet.</param>
        public void Reset(sbyte sourceLink, sbyte destinationLink, bool autoLength = true)
        {
            Clear();

            this.Write((byte)0x02);

            this.Write(sourceLink);
            this.Write(destinationLink);

            if (autoLength)
            {
                this.Write((byte)0x00);
                this.Write((byte)0x00);
            }
        }

        /// <summary>
        /// Finalises the packet by writing the end character and CRC.
        /// </summary>
        public void Finalise()
        {
            this.Write((byte)0x03);

            UInt16 length = (UInt16)(Count + 2);

            this[3] = (byte)(length >> 8);
            this[4] = (byte)(length & 0xFF);

            UInt16 crc = 0;

            for(int i = 0; i < Count; i++)
            {
                crc = CRC16.Crc16(crc, this[i]);
            }

            this.Write(crc);
        }

        /// <summary>
        /// Validates the packet CRC.
        /// </summary>
        /// <returns>If the packet CRC is valid.</returns>
        public bool Validate ()
        {
            var length = GetPacketLength();

            UInt16 crc = 0;
            var packetCrc = GetPacketCRC();

            for (int i = 0; i < length - 2; i++)
            {
                crc = CRC16.Crc16(crc, this[i]);
            }

            return crc == GetPacketCRC();
        }

        /// <summary>
        /// Gets the payload data from the packet.
        /// </summary>
        /// <returns>The payload data.</returns>
        public List<byte> GetPayload()
        {
            return this.Skip(5).Take(GetPacketLength() - 8).ToList();
        }

        /// <summary>
        /// Gets the length of the packet.
        /// </summary>
        /// <returns>The packet length.</returns>
        public UInt16 GetPacketLength()
        {
            return (UInt16)(this[3] << 8 | this[4]);
        }

        /// <summary>
        /// Gets the packet CRC.
        /// </summary>
        /// <returns>The packet CRC</returns>
        public UInt16 GetPacketCRC()
        {
            return (UInt16)(this[GetPacketLength() - 1] << 8 | this[GetPacketLength() - 2]);
        }
    }
}
