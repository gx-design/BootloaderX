﻿namespace UiUtils.Reactive
{
    using Avalonia.Threading;
    using Reactive;
    using ReactiveUI;
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;
    using System.Linq;
    using System.Linq.Expressions;
    using System.Reactive;
    using System.Reactive.Concurrency;
    using System.Reactive.Linq;

    public class PropertyHelper<TRet>
    {
        public PropertyHelper(IReactiveObject source, IObservable<TRet> observable, string propertyName)
        {
            observable.Subscribe(
                v =>
                {
                    source.RaisePropertyChanging(propertyName);
                    Value = v;
                    source.RaisePropertyChanged(propertyName);
                });
        }

        public TRet Value { get; set; }
    }

    public static class ObservableCollectionExtensions
    {
        #region Public methods        

        /// <summary>
        ///     Binds the collection to another one by subscribing to its CollectionChanged event.
        /// </summary>
        /// <typeparam name="T">is the type of items in this collection.</typeparam>
        /// <typeparam name="O">is the type of items in other collection.</typeparam>
        /// <param name="myself">is "this" reference.</param>
        /// <param name="other">is the reference to the collection we are binding to.</param>
        /// <param name="creator">is a function which creates an instance of T, given a reference to O.</param>
        /// <param name="comparer">is a function which compares T to O.</param>
        /// <param name="selector">is a predicate which must evaluate to true for O to be added to this collection.</param>
        public static void BindCollections<T, O>(this ObservableCollection<T> myself, ObservableCollection<O> other,
            Func<O, T> creator, Func<T, O, bool> comparer = null)
        {
            other.CollectionChanged += (sender, e) =>
            {
                switch (e.Action)
                {
                    case NotifyCollectionChangedAction.Add:
                        foreach (O item in e.NewItems)
                        {
                            myself.Insert(other.IndexOf(item), creator(item));
                        }
                        break;

                    case NotifyCollectionChangedAction.Move:
                        throw new NotImplementedException();

                    case NotifyCollectionChangedAction.Remove:
                        // This is O(n^2) but given very small size of collections it is supposed
                        // to be used on, we can live with it.
                        foreach (O item in e.OldItems)
                        {
                            myself.RemoveMatching(t => comparer(t, item));
                        }

                        break;

                    case NotifyCollectionChangedAction.Replace:
                        throw new NotImplementedException();

                    case NotifyCollectionChangedAction.Reset:
                        myself.Clear();
                        break;

                    default:
                        throw new Exception("Unknown action: " + e.Action.ToString());
                }
            };

            if (other.Count > 0)
            {
                foreach (var item in other)
                {
                    myself.Add(creator(item));
                }
            }
        }

        /// <summary>
        ///     Remove all items from the collection for which the predicate evaluates to true.
        ///     Complexity is O(n) so this is only suitable for small collections.
        /// </summary>
        /// <typeparam name="T">is the type of items in this collection.</typeparam>
        /// <param name="myself">is "this" reference.</param>
        /// <param name="condition">is the test to be executed on each collection member.</param>
        public static void RemoveMatching<T>(this ObservableCollection<T> myself, Predicate<T> condition)
        {
            var index = 0;

            while (index < myself.Count)
            {
                if (condition(myself[index]))
                {
                    myself.RemoveAt(index);
                }
                else
                {
                    index++;
                }
            }
        }

        /// <summary>
        ///     Adds an item to the collection. The item is created by the provided creator function
        ///     and then passed to the provided setter function. If creator returns null, the setter will
        ///     not be called and the item will not be added.
        /// </summary>
        /// <typeparam name="T">is the type of the items in this collection.</typeparam>
        /// <param name="myself">is "this" reference.</param>
        /// <param name="creator">is the function to create T.</param>
        /// <param name="setter">is the fuction to initialize T.</param>
        public static void Add<T>(this ObservableCollection<T> myself, Func<T> creator, Action<T> setter)
            where T : class
        {
            var item = creator();

            if (item != null)
            {
                setter(item);

                myself.Add(item);
            }
        }

        /// <summary>
        ///     Adds an item to the collection. The item is created by the provided creator function. If creator
        ///     returns null, the item will not be added.
        /// </summary>
        /// <typeparam name="T">is the type of items in this collection.</typeparam>
        /// <param name="myself">is "this" reference.</param>
        /// <param name="creator">is the function to create T.</param>
        public static void Add<T>(this ObservableCollection<T> myself, Func<T> creator)
            where T : class
        {
            myself.Add(creator, i => { });
        }

        #endregion
    }

    public static class AvaloniaObservableExtensions
    {
        public static PropertyHelper<TRet> ToObservableProperty<TObj, TRet>(
            this IObservable<TRet> This,
            TObj source,
            Expression<Func<TObj, TRet>> property,
            TRet initialValue = default(TRet),
            bool deferSubscription = false,
            IScheduler scheduler = null)
            where TObj : IReactiveObject
        {
            return source.ObservableToProperty(This, property, initialValue, deferSubscription, scheduler);
        }

        public static PropertyHelper<TRet> ObservableToProperty<TObj, TRet>(
                this TObj This,
                IObservable<TRet> observable,
                Expression<Func<TObj, TRet>> property,
                TRet initialValue = default(TRet),
                bool deferSubscription = false,
                IScheduler scheduler = null)
            where TObj : IReactiveObject
        {

            Expression expression = Reflection.Rewrite(property.Body);

            if (expression.GetParent().NodeType != ExpressionType.Parameter)
            {
                throw new ArgumentException("Property expression must be of the form 'x => x.SomeProperty'");
            }

            var name = expression.GetMemberInfo().Name;
            if (expression is IndexExpression)
                name += "[]";

            var result = new PropertyHelper<TRet>(This, observable, name);
            result.Value = initialValue;

            return result;
        }
        
        public static IObservable<TSource> ObserveOnUi<TSource>(this IObservable<TSource> source)
        {
            return new AnonymousObservable<TSource>(observer =>
            {
                return source.Subscribe(
                        x => Dispatcher.UIThread.InvokeAsync(() => observer.OnNext(x)),
                        exception => Dispatcher.UIThread.InvokeAsync(() => observer.OnError(exception)),
                        () => Dispatcher.UIThread.InvokeAsync(() => observer.OnCompleted()));
            });
        }

        public static IObservable<IList<Timestamped<T>>> RollingBuffer<T>(this IObservable<Timestamped<T>> @this, TimeSpan buffering)
        {
            return Observable.Create<IList<Timestamped<T>>>(o =>
            {
                var list = new LinkedList<Timestamped<T>>();

                return @this.Subscribe(tx =>
                {
                    list.AddLast(tx);

                    while (list.First.Value.Timestamp < DateTime.Now.Subtract(buffering))
                    {
                        list.RemoveFirst();
                    }

                    o.OnNext(list.ToList());
                }, ex => o.OnError(ex), () => o.OnCompleted());
            });
        }

    }
}
