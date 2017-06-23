using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gx
{
    public interface IBootloaderApplicationDevice
    {
        Task EnterBootloader();

        Task<bool> Connect();

        bool Detect();

        IObservable<bool> IsAttached { get; }
    }
}
