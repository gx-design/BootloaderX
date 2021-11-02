using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using IntelHexSerializer.File;

namespace BinEncrypt
{
    class Program
    {
        public static ushort Crc16(ushort oldCrc, byte data)
        {
            ushort crc;
            ushort x;

            x = (ushort)(((oldCrc >> 8) ^ data) & 0xff);
            x ^= (ushort)(x >> 4);

            crc = (ushort)((oldCrc << 8) ^ (x << 12) ^ (x << 5) ^ x);

            return crc;
        }

        static UInt32 EncryptDecrypt(UInt32 key, ref UInt32 scrambleKey, UInt32 data)
        {
            var scrambleBytes = BitConverter.GetBytes(scrambleKey);

            ushort crc = (ushort)scrambleKey;

            foreach (var b in scrambleBytes)
            {
                crc = Crc16(crc, b);
            }

            scrambleKey = scrambleKey & 0xFFFF0000;
            scrambleKey |= crc;

            scrambleBytes = BitConverter.GetBytes(scrambleKey);

            crc = (ushort)scrambleKey;

            foreach (var b in scrambleBytes)
            {
                crc = Crc16(crc, b);
            }

            scrambleKey = scrambleKey & 0x0000FFFF;
            scrambleKey |= ((uint)crc) << 16;

            return data ^ key ^ scrambleKey;
        }

        static void PrintInstructions()
        {
            Console.WriteLine("Arguments: [key] [input file] [output file]");
            Console.WriteLine("Key: A hexadecimal 32bit value, must not be 0 or FFFFFFFF");
            Console.WriteLine("InputFile: A path to the input binary file to encrypt (or decrypt)");
            Console.WriteLine("OutputFile: A path to the output file with result.");
        }

        static string ConvertToFullPath(string path)
        {
            if(!Path.IsPathRooted(path))
            {
                return Path.Combine(Directory.GetCurrentDirectory(), path);
            }
            else
            {
                return path;
            }
        }


        static int Main(string[] args)
        {
            if (args.Count() > 3 && args[0] == "--hex-merge")
            {
                Console.WriteLine("Hex Merge");

                IntelHexFile hexFile = null;
                
                for (int i = 1; i < args.Length - 1; i++)
                {
                    var currentFile = ConvertToFullPath(args[i]);

                    if (File.Exists(currentFile))
                    {
                        Console.WriteLine($"Merging: {currentFile}");
                        
                        if (hexFile is null)
                        {
                            hexFile = IntelHexFile.CreateFrom(File.ReadAllText(currentFile));
                        }
                        else
                        {
                            var currentHexFile = IntelHexFile.CreateFrom(File.ReadAllText(currentFile));

                            hexFile.Records.Remove(hexFile.Records.Last());
                            
                            hexFile.Records.AddRange(currentHexFile.Records);
                        }
                    }
                }
                
                File.WriteAllText(ConvertToFullPath(args[^1]), hexFile.ToString());
            }
            else if(args.Count() == 3)
            {
                args[1] = ConvertToFullPath(args[1]);
                args[2] = ConvertToFullPath(args[2]);

                if (!File.Exists(args[1]))
                {
                    PrintInstructions();
                    return -1;
                }

                if (File.Exists(args[2]))
                {
                    File.Delete(args[2]);
                }

                UInt32 myKey = 0;

                try
                {
                    myKey = Convert.ToUInt32(args[0], 16);
                }
                catch(Exception e)
                {
                    PrintInstructions();
                    return -1;
                }

                if(myKey == 0 || myKey == 0xFFFFFFFF)
                {
                    PrintInstructions();
                    return -1;
                }

                var data = File.ReadAllBytes(args[1]);

                var output = File.Create(args[2]);
                
                UInt32 scrambleKey = myKey;

                for (int i = 0; i < data.Length;)
                {
                    UInt32 currentBlock = BitConverter.ToUInt32(data, i);

                    output.Write(BitConverter.GetBytes(EncryptDecrypt(myKey, ref scrambleKey, currentBlock)), 0, 4);
                    i += 4;
                }

                output.Close();

                Console.WriteLine("File Encoded.");
            }
            else
            {
                PrintInstructions();
                return -1;
            }

            return 0;
            
        }
    }
}
