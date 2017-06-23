using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace IntegratedDebugProtocol
{
    public static class ListExtensions
    {
        public static T ReadAt<T>(this List<byte> buffer, int index)
        {
            IntPtr unmanagedPointer = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(T)));
            Marshal.Copy(buffer.ToArray(), index, unmanagedPointer, Marshal.SizeOf(typeof(T)));

            T result = Marshal.PtrToStructure<T>(unmanagedPointer);

            Marshal.FreeHGlobal(unmanagedPointer);

            return result;
        }

        public static string ReadCString(this List<byte> buffer, int index)
        {
            int length = 0;
            int i = index;

            while(buffer[i++] != '\0' && i < buffer.Count)
            {
                length++;
            }

            string result = ASCIIEncoding.ASCII.GetString(buffer.ToArray(), index, length);
            

            return result;
        }

        public static void Write(this List<byte> buffer, bool data)
        {
            buffer.Add(Convert.ToByte(data));
        }

        /// <summary>
        /// Writes a byte to the packet.
        /// </summary>
        /// <param name="data">Data to be written to the packet.</param>
        public static void Write(this List<byte> buffer, byte data)
        {
            buffer.Add(data);
        }

        /// <summary>
        /// Writes a signed byte to the packet.
        /// </summary>
        /// <param name="data">Data to be written to the packet.</param>
        public static void Write(this List<byte> buffer, sbyte data)
        {
            buffer.Add((byte)data);
        }

        /// <summary>
        /// Writes an unsigned 16bit integer to the packet.
        /// </summary>
        /// <param name="data">Data to be written to the packet.</param>
        public static void Write(this List<byte> buffer, UInt16 data)
        {
            buffer.AddRange(BitConverter.GetBytes(data));
        }

        /// <summary>
        /// Writes an unsigned 32bit integer to the packet.
        /// </summary>
        /// <param name="data">Data to be written to the packet.</param>
        public static void Write(this List<byte> buffer, UInt32 data)
        {
            buffer.AddRange(BitConverter.GetBytes(data));
        }

        /// <summary>
        /// Writes a 16bit integer to the packet.
        /// </summary>
        /// <param name="data">Data to be written to the packet.</param>
        public static void Write(this List<byte> buffer, Int16 data)
        {
            buffer.AddRange(BitConverter.GetBytes(data));
        }

        /// <summary>
        /// Writes a 32bit integer to the packet.
        /// </summary>
        /// <param name="data">Data to be written to the packet.</param>
        public static void Write(this List<byte> buffer, Int32 data)
        {
            buffer.AddRange(BitConverter.GetBytes(data));
        }

        /// <summary>
        /// Writes a float to the packet.
        /// </summary>
        /// <param name="data">Data to be written to the packet.</param>
        public static void Write(this List<byte> buffer, float data)
        {
            buffer.AddRange(BitConverter.GetBytes(data));
        }
    }
}
