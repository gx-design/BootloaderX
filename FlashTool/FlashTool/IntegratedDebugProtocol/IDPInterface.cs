using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reactive.Linq;

namespace IntegratedDebugProtocol
{
    public class IdpInterface
    {
        private Action<byte> _state;
        private IdpPacket _parsedPacket;
        private sbyte _packetSourceLink;
        private sbyte _packetDestinationLink;

        public event EventHandler PacketReceived;

        public IObservable<IdpPacket> PacketParsed { get; private set; }     

        public IdpInterface()
        {
            _state = WaitingForStx;
            _parsedPacket = new IdpPacket();

            PacketParsed = Observable.FromEventPattern<EventHandler, EventArgs>(h => PacketReceived += h, h => PacketReceived -= h).Select(ep =>
            {
                return (ep.Sender as IdpInterface).ParsedPacket;
            });
        }

        public IdpPacket ParsedPacket
        {
            get
            {
                return _parsedPacket;
            }
        }

        private void WaitingForStx(byte data)
        {
            if(data == 0x02)
            {
                _state = ReadingSourceLink;
            }
        }

        private void ReadingSourceLink (byte data)
        {
            _packetSourceLink = (sbyte)data;

            _state = ReadingDestinationLink;
        }

        private void ReadingDestinationLink(byte data)
        {
            _packetDestinationLink = (sbyte)data;
            _parsedPacket.Reset(_packetSourceLink, _packetDestinationLink, false);

            _state = ReadingLengthUpper;
        }

        private void ReadingLengthUpper (byte data)
        {
            _parsedPacket.Write(data);

            _state = ReadingLengthLower;
        }

        private void ReadingLengthLower (byte data)
        {
            _parsedPacket.Write(data);

            if (_parsedPacket.GetPacketLength() > 64)
            {
                _state = WaitingForStx;
            }
            else
            {
                _state = WaitingForEtx;
            }
        }

        private void WaitingForEtx (byte data)
        {
            if (_parsedPacket.Count >= _parsedPacket.GetPacketLength() - 3)
            {
                if (data == 0x03)
                {
                    _state = ReadingCRCUpper;
                }
                else
                {
                    _state = WaitingForStx;
                }
            }

            _parsedPacket.Write(data);
        }

        private void ReadingCRCUpper (byte data)
        {
            _parsedPacket.Write(data);
            _state = ReadingCRCLower;
        }

        private void ReadingCRCLower (byte data)
        {
            _parsedPacket.Write(data);

            if(_parsedPacket.Validate())
            {
                PacketReceived?.Invoke(this, new EventArgs());
            }

            _state = WaitingForStx;
        }

        public void ProcessByte(byte data)
        {
            _state(data);
        }

        
    }
}
