namespace IntegratedDebugProtocol.Transactions
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reactive.Subjects;

    public class CommandManager
    {
        private readonly Dictionary<ushort, CommandBase> registeredCommands;
        private readonly Dictionary<Type, CommandBase> registeredPayloadTypes;
        private readonly List<IdpInterface> interfaces;
        private Subject<Transaction> transactions;

        public CommandManager()
        {
            registeredCommands = new Dictionary<ushort, CommandBase>();
            registeredPayloadTypes = new Dictionary<Type, CommandBase>();
            interfaces = new List<IdpInterface>();
            transactions = new Subject<Transaction>();
        }

        public IObservable<Transaction> Transactions
        {
            get
            {
                return transactions;
            }
        }

        public void RegisterCommand<T> () where T : Transaction
        {
            var commandStructure = Activator.CreateInstance<Command<T>>();

            if(registeredCommands.ContainsKey(commandStructure.Payload.Id) || registeredPayloadTypes.ContainsKey(commandStructure.GetType()))
            {
                throw new Exception("The command has already been registed.");
            }

            registeredCommands.Add(commandStructure.Payload.Id, commandStructure);
            registeredPayloadTypes.Add(commandStructure.Payload.GetType(), commandStructure);
        }

        public void RegisterInterface (IdpInterface idpInterface)
        {
            interfaces.Add(idpInterface);

            idpInterface.PacketParsed.Subscribe(o =>
            {
                CommandBase command = null;

                if (registeredCommands.TryGetValue(o.GetPayload().ReadAt<UInt16>(0), out command))
                {
                    transactions.OnNext(command.OnPayloadReceived(o.GetPayload().Skip(2).ToList()));
                }
            });
        }

        public Command<T> GetCommand<T>() where T : Transaction
        {
            Command<T> result = null;

            CommandBase command = null;

            if (registeredPayloadTypes.TryGetValue(typeof(T), out command))
            {
                result = Convert.ChangeType(command, typeof(Command<T>)) as Command<T>;
            }

            return result;
        }
    }

    public abstract class CommandBase
    {
        public CommandBase(Transaction payload)
        {
            Payload = payload;
        }

        internal abstract Transaction OnPayloadReceived(List<byte> payload);

        internal Transaction Payload { get; }
    }

    public class Command<T> : CommandBase where T : Transaction
    {
        public Command() : base(Activator.CreateInstance<T>())
        {

        }

        public event EventHandler<T> CommandReceived;

        public IdpPacket Encode(sbyte sourceLink, sbyte destinationLink, T payload)
        {
            var result = new IdpPacket();
            result.Reset(sourceLink, destinationLink);

            result.Write(payload.Id);

            if (payload.Encode(result))
            {
                result.Finalise();
            }
            else
            {
                result = null;
            }

            return result;
        }

        internal override Transaction OnPayloadReceived(List<byte> payload)
        {
            T result = Activator.CreateInstance<T>();

            if (result.Decode(payload))
            {
                CommandReceived?.Invoke(this, result);
            }

            return result;
        }

        public T CreatePayload()
        {
            return Activator.CreateInstance<T>();
        }
    }
}
