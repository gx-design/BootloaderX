using System;
using System.Collections.Generic;
using System.Text;

namespace Gx
{
    public class VersionInformation
    {
        public float ApplicationVersion { get; set; }
        public string ApplicationReference { get; set; }

        public float BootloaderVersion { get; set; }
        public string BootloaderReference { get; set; }
    }
}
