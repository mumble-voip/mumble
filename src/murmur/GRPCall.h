// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_GRPCALL_H
#define MUMBLE_MURMUR_GRPCALL_H

#include <QtCore/QCoreApplication>
#include <QRandomGenerator>

#include <boost/fiber/all.hpp>
#include <boost/callable_traits/return_type.hpp>
#include <boost/callable_traits/args.hpp>
#include <boost/callable_traits/function_type.hpp>
#include <boost/type_traits/is_detected.hpp>
#include <boost/mp11.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "MurmurRPC.grpc.pb.h"
#pragma GCC diagnostic pop

#include "Server.h"
#include "murmur_grpc/FiberScheduler.h"

#include <atomic>
#include <chrono>
#include <functional>
#include <type_traits>
#include <queue>
#include <tuple>
#include <utility>

class MurmurRPCImpl;

namespace MurmurRPC {
	namespace Wrapper {
		namespace Detail {
			// class designed to 'delete' rpc
			// objects.
			struct rpc_deleter {
				typedef void result_type;

				template<typename T, typename Q = decltype(std::declval<T>().m_alive)>
				void operator()(T* p) const noexcept {
					if (p == nullptr) {
						return;
					}
					p->m_alive.store(false, std::memory_order_release);
					return;
				}
			};

			struct qtimer_deleter {
				typedef void result_type;

				template<typename T>
				void operator()(T* p) const noexcept {
					if (p == nullptr) {
						return;
					}
					p->deleteLater();
					return;
				}
			};

			//tag types for RPC policy objects
			using Unary_t = std::integral_constant<int, 1>;
			using ClientStream_t = std::integral_constant<int, 2>;
			using ServerStream_t = std::integral_constant<int, 3>;
			using BidiStream_t = std::integral_constant<int, 4>;

			struct Unary {
				typedef Unary_t rpctype;
			};
			struct ClientStream {
				typedef ClientStream_t rpctype;
			};
			struct ServerStream {
				typedef ServerStream_t rpctype;
			};
			struct BidiStream {
				typedef BidiStream_t rpctype;
			};

			namespace bf = boost::fibers;
			namespace ct = boost::callable_traits;

			template<typename Stream, typename In, typename Out>
			struct work_queue {
				bf::mutex m_busyMtx;
				bf::fiber m_worker;
				bf::condition_variable m_doWork;
				std::atomic<bool> m_isWorking{false};
				std::queue<bf::packaged_task<void()>> m_workQueue;
				Stream stream;

				work_queue(::grpc::ServerContext *ctx) : stream(ctx){
				}
				~work_queue() = default;

				template<typename Functor>
				bf::future<ct::return_type_t<Functor>> queueWork(Functor && func) {
					static_assert(std::tuple_size<ct::args_t<Functor>>::value == 0,
							"queueWork needs R() signature");

					bf::packaged_task< ct::return_type_t<Functor> () > task(func);
					auto future = task.get_future();
					{
						std::unique_lock< bf::mutex > l(m_busyMtx);
						m_workQueue.emplace(std::move(task));
					}
					m_doWork.notify_one();
					return future;
				}

				void createWorker() {
					m_isWorking.store(true);
					bf::fiber f([&] {
						while(m_isWorking) {
							bf::packaged_task<void ()> task;
							{
								std::unique_lock<bf::mutex> l(m_busyMtx);
								if (m_workQueue.empty()) {
									m_doWork.wait(l, [&](){return !m_workQueue.empty() || !m_isWorking.load();});
									if (!m_isWorking.load()) {
										//drain work queue my just resetting everything
										while (!m_workQueue.empty()) {
											if (m_workQueue.front().valid()) {
												m_workQueue.front().reset();
											}
											m_workQueue.pop();
										}
										return;
									}
								}
								task = std::move(m_workQueue.front());
								m_workQueue.pop();
							}
							if (task.valid()) {
								task();
							}
						}
					});
					m_worker = std::move(f);
					return;
				}

				void cancel() {
					m_isWorking.store(false);
				}

				void done(bool) {
					m_isWorking.store(false);
					m_doWork.notify_one();
					if (m_worker.joinable()) {
						m_worker.join();
					}
				}

				bool writePrivate (const Out& message) {
					bf::promise<bool> okPromise;
					bf::future<bool> okFuture(okPromise.get_future());
					auto l = [okPromise = std::move(okPromise)](bool ok) mutable {
							okPromise.set_value(ok);
					};
					auto cb = ::boost::function<void(bool)>(std::ref(l));
					stream.Write(message, std::addressof(cb));
					auto ret = okFuture.get();
					return ret;
				}

				std::pair< bool, In > readPrivate() {
					In request;
					bf::promise<bool> okPromise;
					bf::future<bool> okFuture(okPromise.get_future());

					auto l = [okPromise=std::move(okPromise)](bool ok) mutable {
									okPromise.set_value(ok);
								};
					auto cb = ::boost::function<void(bool)>(std::ref(l));
					stream.Read(&request, std::addressof(cb));
					auto ret = okFuture.get();
					return std::make_pair(ret, request);
				}

				void error(const ::grpc::Status& err) {
					stream.Finish(err, nullptr);
				}
			};

			template<typename RPCType, template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl {};

			template<template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl<Unary_t, StreamType, In, Out>
			{
				typedef StreamType<Out> Stream;

				In m_Request{};
				Stream stream;

				void error(const ::grpc::Status &err) {
					stream.FinishWithError(err, nullptr);
				}

				void end(const Out& msg) {
					stream.Finish(msg, ::grpc::Status::OK, nullptr);
				}

				void start() {};
				void cancel() {};
				void done(bool) {};

				rpcImpl(::grpc::ServerContext *ctx) : stream(ctx) {
				};
			};

			template<template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl<ServerStream_t, StreamType, In, Out> :
					private work_queue<StreamType<Out>, In, Out> {
				typedef work_queue<StreamType<Out>, In, Out> queue;

				In m_Request{};
				Out m_Response{};
				using queue::stream;

				using queue::done;
				using queue::cancel;
				using queue::error;
				using queue::createWorker;

				template<typename Functor>
				void Write(const Out &msg, Functor && fn) {
					static_assert(std::is_same<ct::function_type_t<Functor>, void(bool)>{},
							"Write(msg, cb)) expects void(bool) for cb");

					(void) this->queueWork([this, msg, fn = std::move(fn)]() mutable -> void {
								auto ok = this->writePrivate(msg);
								static_cast<std::function<void(bool)>>(std::ref(fn))(ok);
							});
				}

				rpcImpl(::grpc::ServerContext *ctx) : work_queue<StreamType<Out>, In, Out>(ctx) {
				};
			};

			template<template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl<BidiStream_t, StreamType, In, Out> :
					private work_queue<StreamType<Out, In>, In, Out> {
				typedef work_queue<StreamType<Out, In>, In, Out> queue;

				using queue::stream;

				using queue::done;
				using queue::cancel;
				using queue::error;
				using queue::createWorker;

				bf::future<bool> write(const Out &message) {
					return this->queueWork([&]() -> bool { return this->writePrivate(message); });
				}

				bf::future<std::pair< bool, In >> read() {
					return this->queueWork([&]() -> std::pair< bool, In > { return this->readPrivate(); });
				}

				bf::future<std::pair< bool, In>> writeRead(const Out &message) {
					return this->queueWork([&]() -> std::pair<bool, In > {
						bool ok = this->writePrivate(message);
						if (!ok) {
							In m;
							return std::make_pair(false, m);
						}
						return this->readPrivate();
					});
				}

				rpcImpl(::grpc::ServerContext *ctx) : work_queue<StreamType<Out, In>, In, Out>(ctx) {
				};
			};
		} //end Detail

		namespace bf = boost::fibers;
		namespace ct = boost::callable_traits;
		using Detail::Unary_t;
		using Detail::ClientStream_t;
		using Detail::ServerStream_t;
		using Detail::BidiStream_t;

		template<typename Derived, typename RPCType = typename Derived::RPCType> class RPCCall : private Derived {
			friend Detail::rpc_deleter;
			friend Derived;

		public:
			typedef typename Derived::InType InType;
			typedef typename Derived::OutType OutType;
			typedef typename Derived::Service ServiceType;
			using Derived::impl;

		private:
			std::atomic<bool> m_isCancelled;
			std::atomic_flag m_finished;
			std::atomic<bool> m_alive;
			std::shared_ptr<RPCCall<Derived>> m_this;

		protected:
			uint32_t m_RPCid;
			::MurmurRPCImpl *rpc;
			ServiceType *service;
			::grpc::ServerContext context;
			Detail::rpcImpl<RPCType, Derived::template StreamType, InType, OutType> impl_detail;
			typedef decltype(impl_detail) ImplType;

		private:

			boost::function<void(bool)> m_DoneFunction;

			void* getDoneFunctionAddr() { return std::addressof(m_DoneFunction); }

			boost::function<void(bool)> m_HandleFunction;

			void* getHandleFunctionAddr() { return std::addressof(m_HandleFunction); }

			bool tryDelete() {
				if(m_alive.load(std::memory_order_acquire)) {
					qDebug("worker %u still alive, but needs deleted", m_RPCid);
					return false;
				}
				delete this;
				return true;
			}

			void finish() {
				if (m_finished.test_and_set()) {
					qDebug("finished called more than once id %u", m_RPCid);
					return;
				}
				qDebug("attempting to delete id %u", m_RPCid);

				// the first delete always fails....
				// but without this in place, it deletes me before i'm ready
				bf::barrier b(2);
				(void) bf::fiber([this, &b](){
					b.wait();
					while (!this->tryDelete()) {
						::boost::this_fiber::sleep_for(std::chrono::milliseconds(100));
					}
				}).detach();
				b.wait();
				m_this.reset();
			}

			void done(bool ok) {
				if (context.IsCancelled()) {
					m_isCancelled.store(true, std::memory_order_release);
					impl_detail.cancel();
				}
				impl_detail.done(ok);
				this->finish();
			}

			template<typename Functor>
			void launchInEventLoop(Functor&& fn) {
				static_assert(std::is_same<ct::function_type_t<Functor>, void()>{},
							"launchInEventLoop(Fn) expects void() for fn");

				boost::fibers::fiber f([ptr = this->getSharedPtr(), func = std::move(fn)](){
					try {
						func();
					} catch (::grpc::Status &ex) {
						if (!ptr->isCancelled()) {
							ptr->error(ex);
						}
					}
				});
				auto& props( f.properties<MurmurRPC::Scheduler::grpc_props>() );
				props.run_in_main(true);
				f.detach();
			}

			template<typename X = RPCType, std::enable_if_t<std::is_same<X, Unary_t>::value>* = nullptr>
			void handle(bool ok);

			template<typename X = RPCType, std::enable_if_t<std::is_same<X, ServerStream_t>::value>* = nullptr>
			void handle(bool ok);

			template<typename X = RPCType, std::enable_if_t<std::is_same<X, BidiStream_t>::value>* = nullptr>
			void handle(bool ok);

		protected:
			template<typename RFn,
				std::enable_if_t<(std::tuple_size<std::add_const_t<ct::args_t<RFn>>>::value == 7)>* = nullptr>
			static void addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc, RFn requestFn, void *handleFn,
					RPCCall<Derived> *call);

			template<typename RFn,
				std::enable_if_t<(std::tuple_size<std::add_const_t<ct::args_t<RFn>>>::value == 6)>* = nullptr>
			static void addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc, RFn requestFn, void *handleFn,
					RPCCall<Derived> *call);

			explicit RPCCall(MurmurRPCImpl *rpcImpl, ServiceType *svc) :
				m_isCancelled(false),
				m_alive(true),
				m_this(static_cast<RPCCall<Derived> *>(this), Detail::rpc_deleter()),
				m_RPCid(QRandomGenerator::global()->generate()),
				rpc(rpcImpl),
				service(svc),
				impl_detail(&context) {
					m_finished.clear();
					this->m_DoneFunction = boost::function<void(bool)>([this](bool ok){ this->done(ok); });
					this->m_HandleFunction = boost::function<void(bool)>([this](bool ok){ this->handle(ok); });
			}

			~RPCCall() {
				qDebug("deleted worker %u", getId());
			}

			std::shared_ptr<RPCCall<Derived>> getSharedPtr() const {
				return m_this;
			}

		public:
			//universal functions
			uint32_t getId() {
				return m_RPCid;
			}

			bool isCancelled() {
				return m_isCancelled.load(std::memory_order_acquire);
			}

			void error(const ::grpc::Status &err) {
				impl_detail.error(err);
			}

			std::weak_ptr<RPCCall<Derived>> getWeakPtr() const {
				return m_this;
			}

			//unary streams
			template<typename Impl = ImplType>
			auto end(const OutType &message = OutType())
					-> decltype(std::declval<Impl>().end(OutType())) {
				impl_detail.end(message);
			}

			// functions for client streams
			template<typename Impl = ImplType, typename Functor>
			auto Write(const OutType& msg, Functor&& func)
					-> decltype(std::declval<Impl>().Write(OutType(), std::declval<Functor>())) {
				return impl_detail.Write(msg, std::forward<Functor>(func));
			}

			// stream-stream functions
			template<typename MsgType>
			auto write(MsgType message) {
				return impl_detail.queueWork([&]() -> bool { return impl_detail.writePrivate(message); });
			}

			template<typename Impl = ImplType>
			auto read() -> decltype(std::declval<Impl>().read()) {
				return impl_detail.read();
			}

			template<typename Impl = ImplType>
			auto writeRead(const OutType& message) -> decltype(std::declval<Impl>().writeRead(OutType())) {
				return impl_detail.writeRead(message);
			}

			static void create(MurmurRPCImpl *rpc, ServiceType *service) {
				auto call = new RPCCall<Derived>(rpc, service);
				auto doneFn = call->getDoneFunctionAddr();
				call->context.AsyncNotifyWhenDone(doneFn);
				auto handleFn = call->getHandleFunctionAddr();
				auto requestFn = Derived::getRequestFn();
				addCQHandler(service, rpc, requestFn, handleFn, call);
			}
		};
} //end Wrapper
} //end MurmurRPC

#include "MurmurRPC.proto.Wrapper.cpp"

#endif

#ifdef MUMBLE_MURMUR_GRPC_WRAPPER_IMPL
#ifndef MUMBLE_MURMUR_GRPC_WRAPPER_IMPL_HH_
#define MUMBLE_MURMUR_GRPC_WRAPPER_IMPL_HH_



namespace MurmurRPC {
	namespace Wrapper {

		using Detail::Unary_t;
		using Detail::ClientStream_t;
		using Detail::ServerStream_t;
		using Detail::BidiStream_t;

		namespace ct = boost::callable_traits;

		template<typename Derived, typename RPCType>
		template<typename RFn, std::enable_if_t<(std::tuple_size<std::add_const_t<ct::args_t<RFn>>>::value == 7)>*>
		void RPCCall<Derived, RPCType>::addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc,
					RFn requestFn, void *handleFn, RPCCall<Derived> *call) {
				(svc->*requestFn)(&call->context, &call->impl_detail.m_Request, &call->impl_detail.stream, rpc->m_completionQueue.get(), rpc->m_completionQueue.get(), handleFn);
		}

		template<typename Derived, typename RPCType>
		template<typename RFn, std::enable_if_t<(std::tuple_size<std::add_const_t<ct::args_t<RFn>>>::value == 6)>*>
		void RPCCall<Derived, RPCType>::addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc,
					RFn requestFn, void *handleFn, RPCCall<Derived> *call) {
			(svc->*requestFn)(&call->context, &call->impl_detail.stream, rpc->m_completionQueue.get(),
					rpc->m_completionQueue.get(), handleFn);
		}

		template<typename Derived, typename RPCType>
		template<typename X, std::enable_if_t<std::is_same<X, Unary_t>::value>*>
		void RPCCall<Derived, RPCType>::handle(bool /*unused*/) {
			RPCCall<Derived>::create(this->rpc, this->service);
			auto ptr = this->getSharedPtr();
			this->launchInEventLoop([this, ptr](){
					ptr->impl(ptr, this->impl_detail.m_Request);
			});
		}

		template<typename Derived, typename RPCType>
		template<typename X, std::enable_if_t<std::is_same<X, ServerStream_t>::value>*>
		void RPCCall<Derived, RPCType>::handle(bool /*unused*/) {
			RPCCall<Derived>::create(this->rpc, this->service);
			auto ptr = this->getSharedPtr();
			this->launchInEventLoop([this, ptr](){
				ptr->impl(ptr, this->impl_detail.m_Request);
			});
			impl_detail.createWorker();
		}

		template<typename Derived, typename RPCType>
		template<typename X, std::enable_if_t<std::is_same<X, BidiStream_t>::value>*>
		void RPCCall<Derived, RPCType>::handle(bool /*unused*/) {
			RPCCall<Derived>::create(this->rpc, this->service);
			auto ptr = this->getSharedPtr();
			this->launchInEventLoop([this, ptr](){
				ptr->impl(ptr);
			});
			impl_detail.createWorker();
		}
	} //end Wrapper
} //end MurmurRPC

namespace MurmurRPC {
	namespace Wrapper {
	namespace Container {

		namespace mp11 = boost::mp11;

		template<typename C>
		template<typename It>
		auto weakContainer<C>::getLockedRange(const std::pair<It, It>& rng) {
			auto range = ::boost::iterator_range<It>(rng.first, rng.second);
			auto locked = ::boost::adaptors::transform(range, locked_adapter());
			return ::boost::remove_if<::boost::return_begin_found>(locked, locked_filter());
		}

		template<typename C>
		template<typename Idx>
		auto weakContainer<C>::getLockedIndex(const Idx&) {
			std::lock_guard<decltype(m_Mtx)> lk(m_Mtx);
			const auto& idx = mi::get<Idx>(container);
			return getLockedRange(std::make_pair(idx.cbegin(), idx.cend()));
		}

		template<typename C>
		template<typename Functor>
		auto weakContainer<C>::getLocked(Functor&& func) {
			using return_type = std::decay_t<decltype(std::declval<Functor>()(*this))>;
			using voids = mp11::mp_transform<mp11::mp_void, return_type>;
			static_assert(std::is_same<voids, std::pair<void, void>>::value && 
					mp11::mp_same<return_type>::value , "func must have signature std::pair<Iter, Iter>(&Container)");
			std::lock_guard<decltype(m_Mtx)> lk(m_Mtx);
			const auto& range = func(*this);
			return this->getLockedRange(range);
		}
	}
}}

#include "MurmurRPC.proto.Wrapper.cpp"

#endif
#endif
