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
#include <boost/type_traits.hpp>
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

			/// \brief Custom deleter for RPCCall objects.
			///
			/// Instead of deleting the objects, just sets RPCCall::m_alive to false
			///
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

			/// \brief Custom deleter for QTimer.
			///
			/// If you try to delete a QTimer allocated with new, you will
			/// find it throws an exception. This deleter just calls the
			/// method to delete the timer on the next event loop run
			///
			struct qtimer_deleter {
				using result_type = void;

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
				using rpctype = Unary_t;
			};
			struct ClientStream {
				using rpctype = ClientStream_t;
			};
			struct ServerStream {
				using rpctype = ServerStream_t;
			};
			struct BidiStream {
				using rpctype = BidiStream_t;
			};

			namespace bf = boost::fibers;
			namespace ct = boost::callable_traits;

			/// \brief Helper class to implement a work queue running in a seperate fiber.
			///
			/// This class takes in arbitrary functions, packages them
			/// into `boost::fibers::packaged_task<>`, and then returns a
			/// future to wait on. It then runs each function in the order
			/// recieved.
			///
			/// \tparam Stream typename of the steram object used for communications
			/// \tparam In typename of the protobuf messages that will be recieved
			/// \tparam Out typename of the protobuf messages that will be sent
			///
			template<typename Stream, typename In, typename Out>
			struct work_queue {
				bf::mutex m_busyMtx;
				bf::fiber m_worker;
				bf::condition_variable m_doWork;
				std::atomic<bool> m_isWorking{false};
				std::queue<bf::packaged_task<void()>> m_workQueue;
				Stream stream;

				/// \brief Constructor.
				///
				/// You cannot send messages until createWorker() has been called
				///
				/// \param [in] ctx a `grpc::ServerContext` to create the stream with
				///
				work_queue(::grpc::ServerContext *ctx) : stream(ctx){
				}

				~work_queue() = default;

				/// \brief Queues a function to be run in the work queue
				///
				/// queueWork takes any arbitrary functor (llambda, function object,
				/// function ponter) puts it into a `boost::fibers::packaged_task<>` and returns
				/// the `boost::fibers::future<>` that is associated with the outcome
				/// of the functor.
				///
				/// It then queues the work to be done in the worker fiber.
				///
				/// \param func [in] a *MoveConstructable* functor with no arguments
				/// \return a `boost::fibers::future<R>` where `R` is the return value
				/// of the functor that was given
				///
				template<typename Functor>
				auto queueWork(Functor&& func) -> bf::future<ct::return_type_t<Functor>> {
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

				/// \brief Initializes the work queue.
				///
				/// createWorker spawns a new fiber in the current thread that
				/// is responsible for running the tasks. When there are no tasks,
				/// it will wait on a condition variable until queueWork(Functor&& func)
				/// is called.
				///
				/// cancel() may be called to cancel the processing of work early, but
				/// does not remove any jobs from the queue, nor alert the worker fiber
				/// that anything has happened.
				///
				/// done(bool) cancels the processing of work, alerts the worker fiber to
				/// return from sleeping, and if the fiber is still joinable, join
				/// the fiber.
				///
				/// The worker fiber is stored in \ref m_worker. Since this fiber is not
				/// detached it is critical to call done(bool) before destroying the object.
				///
				auto createWorker() -> void {
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

				/// cancels processing of work, but does not wake up the worker fiber
				void cancel() {
					m_isWorking.store(false);
				}

				/// \brief cancels processing of work, then joins the worker fiber
				///
				/// This function must be called before destroying the object, as
				/// if the worker fiber is still joinable when destroyed,
				/// `std::terminate` will be called.
				///
				void done(bool) {
					m_isWorking.store(false);
					m_doWork.notify_one();
					if (m_worker.joinable()) {
						m_worker.join();
					}
				}

				/// \brief implementation detail not to be called outside the framework
				///
				/// This is one of the core functions used to communicate with the
				/// gRPC framework. It takes a message to be written, calls
				/// the write function, and then blocks until the completion queue
				/// calls the callback with a `bool` to indicate sucesss.
				///
				/// \param [in] message a const lref to the protobuf message to write.
				/// this function does not take ownership of the written message.
				/// \return `true` or `false` depending on what the completion queue
				/// returns when the callback sent to be called
				///
				bool writePrivate (const Out& message) {
					bf::promise<bool> okPromise;
					bf::future<bool> okFuture(okPromise.get_future());
#ifdef BOOST_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
					auto l = std::bind([](bf::promise<bool>& okPromise, bool ok) -> void {
							okPromise.set_value(ok);
					}, std::move(okPromise), std::placeholders::_1);
					std::function<void(bool)> cb = std::ref(l);
#else
					auto l = [okPromise = std::move(okPromise)](bool ok) mutable {
							okPromise.set_value(ok);
					};
					auto cb = std::function<void(bool)>(std::ref(l));
#endif
					stream.Write(message, std::addressof(cb));
					auto ret = okFuture.get();
					return ret;
				}

				/// \brief implementation detail not to be called outside the framework
				///
				/// This is one of the core functions used to communicate with the
				/// gRPC framework. It reads a message from the stream, blocking
				/// until it has been recieved.
				///
				/// \return `std::pair<bool, Message>` where bool is the sucess code
				/// and message is either blank or the recieved message
				///
				std::pair< bool, In > readPrivate() {
					In request;
					bf::promise<bool> okPromise;
					bf::future<bool> okFuture(okPromise.get_future());

#ifdef BOOST_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
					auto l = std::bind([](bf::promise<bool>& okPromise, bool ok) -> void {
							okPromise.set_value(ok);
					}, std::move(okPromise), std::placeholders::_1);
					std::function<void(bool)> cb = std::ref(l);
#else
					auto l = [okPromise = std::move(okPromise)](bool ok) mutable {
							okPromise.set_value(ok);
					};
					auto cb = std::function<void(bool)>(std::ref(l));
#endif
					stream.Read(&request, std::addressof(cb));
					auto ret = okFuture.get();
					return std::make_pair(ret, request);
				}

				/// \brief implementation detail not to be called outside the framework
				///
				/// finishes the stream with the given error code and
				/// returns without waiting
				///
				void error(const ::grpc::Status& err) {
					stream.Finish(err, nullptr);
				}
			};

			template<typename RPCType, template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl {};

			/// \brief implentation detail for unary streams
			///
			/// This is the helper class for unary streams where a message
			/// is recieved and either a message is sent out or an error is sent out
			///
			/// \tparam StreamType template for creating the stream object
			/// \tparam In typename of the incoming protobuf messages
			/// \tparam Out typename of the outgoing protobuf messages
			///
			template<template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl<Unary_t, StreamType, In, Out>
			{
				typedef StreamType<Out> Stream;

				In m_Request{};
				Stream stream;

				/// \brief Request the stream be finished with an error code, then returns
				///
				/// \param err the error code to be sent
				///
				void error(const ::grpc::Status &err) {
					stream.FinishWithError(err, nullptr);
				}

				/// \brief Requests an output message to be written, then returns
				///
				/// \param msg const lref to the output message. Ownership
				/// is not taken of the message and it can destroyed after
				/// this returns
				///
				void end(const Out& msg) {
					stream.Finish(msg, ::grpc::Status::OK, nullptr);
				}

				/// does nothing, but needed to satisfy interface
				void start() {};


				/// does nothing, but needed to satisfy interface
				void cancel() {};

				/// does nothing, but needed to satisfy interface
				void done(bool) {};

				/// \brief Constructor.
				///
				/// \param ctx the grpc::ServerContext to create the stream
				///
				rpcImpl(::grpc::ServerContext *ctx) : stream(ctx) {
				};
			};

			/// \brief Helper class for streams where there is one request, and many output messages
			///
			/// This class depends on \ref work_queue
			/// for most of its implementation.
			///
			/// \tparam StreamType template of the stream object used for writing
			/// \tparam In typename of the incoming protobuf messages
			/// \tparam Out typename of the outgoing protobuf messages
			///
			template<template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl<ServerStream_t, StreamType, In, Out> :
					private work_queue<StreamType<Out>, In, Out> {
				using queue = work_queue<StreamType<Out>, In, Out>;

				In m_Request{};
				Out m_Response{};
				using queue::stream;

				using queue::done;
				using queue::cancel;
				using queue::error;
				using queue::createWorker;

				/// \brief Requests write of message and returns immediately.
				/// fn is called on completion of write.
				///
				/// This is the main function used by ServerStream type RPC calls. It asks
				/// for a message to be written and returns immediately, afterward it calls a
				/// user-defined functor with the success or error of the write. This functor
				/// will be called in the thread of the work queue.
				///
				/// Since no two calls to the gRPC Write function at the same time are allowed,
				/// the work queue is used to ensure that they do not overlap.
				///
				/// \param [in] msg protobuff message to be written. Ownership is not taken
				/// \param [in] fn a *MoveConstructable* functor of signature `void(bool)` to
				/// be called after the write has been completed. Ownership is taken of this
				/// functor and it cannot be used after this call. It is expected to be stack
				/// allocated.
				///
				template<typename Functor>
				void Write(const Out &msg, Functor && fn) {
					static_assert(std::is_same<ct::function_type_t<Functor>, void(bool)>{},
							"Write(msg, cb)) expects void(bool) for cb");

#ifdef BOOST_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
					this->queueWork(
						std::function<void()>(
							std::bind(
								[this, msg](Functor& func) -> void {
									auto ok = this->writePrivate(msg);
									static_cast<std::function<void(bool)>>(std::ref(func))(ok);
								},
								std::move(fn)
							)
						)
					);
#else
					(void) this->queueWork([this, msg, fn = std::move(fn)]() mutable -> void {
								auto ok = this->writePrivate(msg);
								static_cast<std::function<void(bool)>>(std::ref(fn))(ok);
							});
#endif
				}

				/// \brief Constructor.
				///
				/// \param ctx the grpc::ServerContext used to create the communication stream
				///
				rpcImpl(::grpc::ServerContext *ctx) : work_queue<StreamType<Out>, In, Out>(ctx) {
				};
			};

			/// \brief helper class for bidirectional stream RPC calls
			///
			/// Most of the implementation of this class is in \ref work_queue
			///
			/// This is the helper for bidirectional streams, where both the server
			/// and client will be sending messages back and forth, such as
			/// textMessageFilter. As such, it exposes much more of the boost::fibers
			/// implementation that is used by all RPC types.
			///
			/// \tparam StreamType template of the stream type that will be used for messages
			/// \tparam In typename of the incoming protobuf messages
			/// \tparam Out typename out outgoing prototobuf messages
			///
			template<template<typename...> class StreamType, typename In, typename Out>
			struct rpcImpl<BidiStream_t, StreamType, In, Out> :
					private work_queue<StreamType<Out, In>, In, Out> {

				using queue = work_queue<StreamType<Out, In>, In, Out>;

				using queue::stream;

				/// see \ref work_queue::done()
				using queue::done;

				/// see \ref work_queue::cancel()
				using queue::cancel;

				/// see \ref work_queue::error()
				using queue::error;

				/// see \ref work_queue::createWorker()
				using queue::createWorker;

				/// \brief Asks message to be written.
				///
				/// It enqueues a message to be written into the work queue. Since no
				/// two messages can be written at the same time if the futures are not requested
				/// in the order in which they were recieved a single same fiber, deadlock can result.
				///
				/// Futures can be requested out of order as long as different fibers request them;
				/// they will just be woken up in the same order in which the writes were requested.
				///
				/// Calling `get()` or `wait()` on the returned future will block that fiber until
				/// the write has been completed. If an exception was encountered or the promise was
				/// destroyed `get()` will throw an exception. See the `boost::fibers` documentation
				/// for more details
				///
				/// \param [in] message the message to be written. Ownership is not taken and it can
				/// be destroyed after this call
				/// \return `boost::fibers::future<bool>` that will contain the result of the write.
				///
				bf::future<bool> write(const Out& message) {
					return this->queueWork([&]() -> bool { return this->writePrivate(message); });
				}

				/// \brief Asks message to be read.
				///
				/// It enqueues a read request in the work queue. It will then
				/// return immediately with a future that will both have the success of the read
				/// and the read message or a blank message.
				///
				/// See write() for more info and some warnings about reading futures.
				///
				/// \return `boost::future<std::pair<bool,Message>>` with the bool being the success
				/// and the message being the read message or a blank one on failure.
				///
				bf::future<std::pair< bool, In >> read() {
					return this->queueWork([&]() -> std::pair< bool, In > { return this->readPrivate(); });
				}

				/// \brief Asks for write of message and read of response, atomically.
				///
				/// This helper makes sure that if you want the response to your
				/// outgoing message, no other call to read() will intervene.
				///
				/// It enqueues a task that both writes a message and reads a response. Due to the
				/// nature of the the gRPC and the work queue, just calling write()
				/// waiting on the result, and afterward calling read() would not promise that the
				/// read message was the reply to your outgoing one. This is because read() could
				/// be called during your wait for the write, and gRPC reads whatever message comes
				/// next. Even calling write() and then calling read() and
				/// waiting on the futures in order would not be safe the RPC object can
				/// be called by multiple threads, as there could be a read() call put in before
				/// yours.
				///
				/// By making the write and read both one task, it ensures that read reply
				/// is the next message recieved after the write is completed. Similar to 
				/// read() it returns a future with the result of the read and the read message.
				///
				/// See write() for warnings about reading futures.
				///
				/// \param message the protobuf message to be written. Ownership is not taken
				/// and it can be destroyed after this returns.
				/// \return `boost::fibers::future<std::pair<bool,Message>>` if the first item
				/// in the pair is false, either the write or read failed, and the message is blank.
				/// There is not a way to determine if the write or the read was the failure
				///
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

				/// \brief Constructor.
				///
				/// \param ctx grpc::ServerContext to be used to create the stream write/read
				///
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

		/// \brief RPCCall class template. Makes concrete RPCCall objects that can be used.
		///
		/// This class template uses the Derived type to generate an RPCall object that
		/// abstracts away the gRPC interface being used. Clients are expected to use
		/// `std::shared_ptr` and `std::weak_ptr` for calling members and container storange
		/// as the lifecycle of the class is determined by gRPC itself.
		///
		/// To ensure that we can erase ourself safely, we hold onto \ref m_this which
		/// is a shared pointer with a special deleter that tells us we are safe to delete.
		/// All other smart pointers to this are created from this pointer, which means that
		/// each one of them shares our custom deleter. By using smart pointers both as the
		/// only way to access the object we use `std::smart_ptr` as our own reference counter.
		///
		/// Derived is expected to model \link codegen.h one of serveral types of structs.\endlink
		///
		/// \tparam Derived a struct generated by the gRPC codegen which has the
		/// type definitions and a few static implementation function that
		/// control behavior when a new service request is recieved that need to
		/// be written by the implementor of the call.
		///
		template<typename Derived, typename RPCType = typename Derived::RPCType> class RPCCall : private Derived {
			friend Detail::rpc_deleter;
			friend Derived;

		public:
			using InType = typename Derived::InType; ///< typedef for incoming protobuf message type
			using OutType =  typename Derived::OutType; ///< typedef for outgoing protobuf message type
			using ServiceType = typename Derived::Service; ///< typedef for the service we implement, always MurmurRPC::V1::AsyncService at present


		private:
			std::atomic<bool> m_isCancelled; ///< flag indicating we were cancelled
			std::atomic_flag m_finished; ///< used to prevent double calls to finish, likely unneeded in current design

			/// The custom deleter of \ref m_this sets this to true when it was supposed to delete us
			std::atomic<bool> m_alive;

			/// \brief `std::shared_ptr` with customized deleter.
			///
			/// This should be the source of all smart pointers to this object, all
			/// of them will then share the same deleter. The deleter simply sets
			/// #m_alive to false, letting us know there are no more references left.
			///
			std::shared_ptr<RPCCall<Derived>> m_this;

			uint32_t m_RPCid; ///< unique id for this object, used for lookup in containers
			::MurmurRPCImpl *rpc; ///< ptr to MurmurRPCImpl that we are in
			ServiceType *service; ///< ptr to the Service we are in
			::grpc::ServerContext context; ///< the unique context for our call

			///
			/// This is helper where most of our implementation determined by the
			/// type of stream exist.
			///
			/// see:
			/// * \ref Detail::rpcImpl<Unary_t,StreamType,In,Out> "rpcImpl<Unary_t>"
			/// * \ref Detail::rpcImpl<ServerStream_t,StreamType,In,Out> "rpcImpl<ServerStream_t>
			/// * \ref Detail::rpcImpl<BidiStream_t,StreamType,In,Out> "rpcImpl<BidiStream_t>"
			///
			Detail::rpcImpl<RPCType, Derived::template StreamType, InType, OutType> impl_detail;
			using ImplType = decltype(impl_detail);

			std::function<void(bool)> m_DoneFunction = [this](bool ok){ this->done(ok);};

			/// \brief Gets the address of the callback used when gRPC tells us we are done
			/// and can be deleted
			void* getDoneFunctionAddr() { return std::addressof(m_DoneFunction); }

			std::function<void(bool)> m_HandleFunction = [this](bool ok){ this->handle(ok); };

			/// Gets the address of the function used to handle request for a new service
			void* getHandleFunctionAddr() { return std::addressof(m_HandleFunction); }

			/// brings Derived's implementation method into this class
			using Derived::impl;

			///
			/// Attempts to delete ourself, but will return failure if other objects
			/// still contain valid references to us.
			///
			/// \return sucess or failure of deletion
			///
			bool tryDelete() {
				if(m_alive.load(std::memory_order_acquire)) {
					qDebug("worker %u still alive, but needs deleted", m_RPCid);
					return false;
				}
				delete this;
				return true;
			}

			///
			/// this is essentially the destructor for the object. After gRPC lets us know
			/// that we are done, this is the last method called. It spawns a new
			/// fiber that will repeatedly call tryDelete() until it succeeds
			/// after all references are gone. It then resets \ref m_this so no new
			/// valid pointers can be obtained. After all other copies of \ref m_this
			/// are also deleted, we can safely `delete this`
			///
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
						boost::this_fiber::sleep_for(std::chrono::milliseconds(100));
					}
				}).detach();
				b.wait();
				m_this.reset();
			}

			///
			/// called by gRPC when this object is either finished or cancelled.
			/// sets m_isCancelled if we are cancelled. Then it calls the user-defined
			/// function, calls the impl_detail.done() and finally calls finish() to
			/// destroy ourself
			///
			void done(bool ok) {
				if (context.IsCancelled()) {
					m_isCancelled.store(true, std::memory_order_release);
					impl_detail.cancel();
				}
				impl_detail.done(ok);
				this->finish();
			}

			/// \brief Helper function to run some functor in the main event loop of murmur.
			///
			/// This replaces the RPCExecEvent callback that was used in
			/// the old method. At the moment, only the Derived::impl() method is
			/// called here. To match the old interface, those functions can throw
			/// `grpc::Status` like it was an exception to indicate an error.
			///
			/// If an `grpc::Status` is caught, if the service was not cancelled, it is
			/// used to call error() which will notify gRPC of the failure and close
			/// the connection.
			///
			/// \param fn a *MoveConstructable* function with signature `void()` to be
			/// run in the main murmur event loop. Ownership will be taken of the function.
			///
			template<typename Functor>
			void launchInEventLoop(Functor&& fn) {
				static_assert(std::is_same<ct::function_type_t<Functor>, void()>{},
							"launchInEventLoop(Fn) expects void() for fn");

#ifdef BOOST_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
				boost::fibers::fiber f(
					std::bind([](decltype(m_this) ptr, Functor func) {
						try {
							func();
						} catch (::grpc::Status &ex) {
							if (!ptr->isCancelled()) {
								ptr->error(ex);
							}
						}
					}, std::move(this->getSharedPtr()), std::move(fn)));
#else
				boost::fibers::fiber f([ptr = this->getSharedPtr(), func = std::move(fn)](){
					try {
						func();
					} catch (::grpc::Status &ex) {
						if (!ptr->isCancelled()) {
							ptr->error(ex);
						}
					}
				});
#endif
				auto& props( f.properties<MurmurRPC::Scheduler::grpc_props>() );
				props.run_in_main(true);
				f.detach();
			}

			/// \brief function called by gRPC when a new service is requested.
			///
			/// Creates a new identical
			/// RPCall to handle the next new service request. It then acts somewhat differently
			/// depending on the type of service.
			///
			/// For unary services, the incoming message has already been read in. It then calls
			/// Derived::impl() in the main event loop with the message and a pointer to us for the end(). 
			///
			/// For server stream services, in request message has already been read in. This
			/// will then request to call Derived::impl() in the main event loop with the message
			/// and a pointer to us. The typical implementation just checks the validity of the request,
			/// and then adds a `std::weak_ptr`, along with some other information into a container
			/// of listening RPC clients. When that call finishes, the work queue is started so that
			/// outgoing messages can be written.
			///
			/// For bidirectional streams, message has not been read in. It requests Detail::impl() be
			/// called in the main loop and starts the work queue for writes and reads. The typical
			/// implementation reads in the message, checks validity, then adds a `std::weak_ptr` along
			/// with other details to a container of listeners.
			///
			template<typename X = RPCType, boost::enable_if_t<std::is_same<X, Unary_t>::value>* = nullptr>
			void handle(bool ok);

			template<typename X = RPCType, boost::enable_if_t<std::is_same<X, ServerStream_t>::value>* = nullptr>
			void handle(bool ok);

			template<typename X = RPCType, boost::enable_if_t<std::is_same<X, BidiStream_t>::value>* = nullptr>
			void handle(bool ok);

			/// \brief Helper to add ourself to the completion queue.
			///
			/// We need to request to be added to the
			/// queue at startup and whenever a new service is requested (to replace the old one).
			///
			/// This function has a template with two different calls as the call to add to the queue
			/// has a slightly different signature for Bidirectional streams than for Unary and Server
			/// streams.
			///
			template<typename RFn,
				boost::enable_if_t<(std::tuple_size<boost::add_const_t<ct::args_t<RFn>>>::value == 7)>* = nullptr>
			static void addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc, RFn requestFn, void *handleFn,
					RPCCall<Derived> *call);

			template<typename RFn,
				boost::enable_if_t<(std::tuple_size<boost::add_const_t<ct::args_t<RFn>>>::value == 6)>* = nullptr>
			static void addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc, RFn requestFn, void *handleFn,
					RPCCall<Derived> *call);

			/// \brief Constructor.
			///
			/// Only to be called in create(). Setups up our flags,
			/// m_this, generates a random number to be used as an id, and creates
			/// the impl_detail object which has most of the code used by outside classes
			///
			explicit RPCCall(MurmurRPCImpl *rpcImpl, ServiceType *svc) :
				m_isCancelled(false),
				m_alive(true),
				m_this(static_cast<RPCCall<Derived> *>(this), Detail::rpc_deleter()),
				m_RPCid(QRandomGenerator::global()->generate()),
				rpc(rpcImpl),
				service(svc),
				impl_detail(&context) {
					m_finished.clear();
			}

			~RPCCall() {
				qDebug("deleted worker %u", getId());
			}

			std::shared_ptr<RPCCall<Derived>> getSharedPtr() const {
				return m_this;
			}

		public:
			/// \brief Gets the unique id associated with this RPCCall.
			///
			/// Used in containers as they contain `std::weak_ptr` which you can't compare against.
			///
			uint32_t getId() {
				return m_RPCid;
			}

			/// \brief Checks if this call has been cancelled.
			///
			/// Normally this would be done by the client disconnecting
			///
			bool isCancelled() {
				return m_isCancelled.load(std::memory_order_acquire);
			}

			/// \brief Cancels the stream with an error code
			///
			/// \param err grpc::Status with the error in it
			///
			void error(const ::grpc::Status &err) {
				impl_detail.error(err);
			}

			/// \brief Gets a `std::weak_ptr` copied from \ref m_this.
			///
			/// This can be used to store an identifier for
			/// a listener in event containers. Since it does not have ownership, and
			/// we can delete ourself at any time gRPC asks us to, others
			/// accessing the container will never be left holding onto a bad reference.
			///
			std::weak_ptr<RPCCall<Derived>> getWeakPtr() const {
				return m_this;
			}

			/// \brief only available for unary streams
			///
			/// Called at the end of the `Detail::impl()` function to send the outgoing request.
			///
			/// This is somewhat bad design as if `impl()` fails to call this or
			/// throw an exception we are left dangling until we get cancelled due
			/// to a timeout. This is a likely non-method in the future.
			///
			template<typename Impl = ImplType>
			auto end(const OutType &message = OutType())
					-> decltype(std::declval<Impl>().end(OutType())) {
				impl_detail.end(message);
			}

			/// \brief only for server stream types.
			///
			/// \copydoc Detail::rpcImpl<ServerStream_t,StreamType,In,Out>::Write()
			///
			template<typename Impl = ImplType, typename Functor>
			auto Write(const OutType& msg, Functor&& func)
					-> decltype(std::declval<Impl>().Write(OutType(), std::declval<Functor>())) {
				return impl_detail.Write(msg, std::forward<Functor>(func));
			}

			/// \brief only for bidirection streams.
			///
			/// \copydoc Detail::rpcImpl<BidiStream_t,StreamType,In,Out>::write()
			///
			template<typename Impl = ImplType, typename MsgType>
			auto write(MsgType message)
				-> decltype(std::declval<Impl>().queueWork(std::declval<bool(*)()>())) {
				return impl_detail.queueWork([&]() -> bool { return impl_detail.writePrivate(message); });
			}

			/// \brief only for bidirection streams
			///
			/// \copydoc Detail::rpcImpl<BidiStream_t,StreamType,In,Out>::read()
			///
			template<typename Impl = ImplType>
			auto read() -> decltype(std::declval<Impl>().read()) {
				return impl_detail.read();
			}

			/// \brief only for bidirection streams
			///
			/// \copydoc Detail::rpcImpl<BidiStream_t,StreamType,In,Out>::writeRead()
			///
			template<typename Impl = ImplType>
			auto writeRead(const OutType& message) -> decltype(std::declval<Impl>().writeRead(OutType())) {
				return impl_detail.writeRead(message);
			}

			///
			/// This method should be called only one time during initialization
			/// to create the service. It is also called to create new services
			/// when old ones get taken.
			///
			/// It creates a new service, sets up the callbacks for both
			/// the function to handle a request and completion, and uses
			/// them to add this to the completion queue.
			///
			/// \param rpc pointer to MururGRPCImpl
			/// \param service pointer to ServiceType, likely MurmurRPC::V1::AsyncService
			///
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
		template<typename RFn, boost::enable_if_t<(std::tuple_size<boost::add_const_t<ct::args_t<RFn>>>::value == 7)>*>
		void RPCCall<Derived, RPCType>::addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc,
					RFn requestFn, void *handleFn, RPCCall<Derived> *call) {
				(svc->*requestFn)(&call->context, &call->impl_detail.m_Request, &call->impl_detail.stream, rpc->m_completionQueue.get(), rpc->m_completionQueue.get(), handleFn);
		}

		template<typename Derived, typename RPCType>
		template<typename RFn, boost::enable_if_t<(std::tuple_size<boost::add_const_t<ct::args_t<RFn>>>::value == 6)>*>
		void RPCCall<Derived, RPCType>::addCQHandler(ServiceType* svc, MurmurRPCImpl* rpc,
					RFn requestFn, void *handleFn, RPCCall<Derived> *call) {
			(svc->*requestFn)(&call->context, &call->impl_detail.stream, rpc->m_completionQueue.get(),
					rpc->m_completionQueue.get(), handleFn);
		}

		template<typename Derived, typename RPCType>
		template<typename X, boost::enable_if_t<std::is_same<X, Unary_t>::value>*>
		void RPCCall<Derived, RPCType>::handle(bool /*unused*/) {
			RPCCall<Derived>::create(this->rpc, this->service);
			auto ptr = this->getSharedPtr();
			this->launchInEventLoop([this, ptr](){
					ptr->impl(ptr, this->impl_detail.m_Request);
			});
		}

		template<typename Derived, typename RPCType>
		template<typename X, boost::enable_if_t<std::is_same<X, ServerStream_t>::value>*>
		void RPCCall<Derived, RPCType>::handle(bool /*unused*/) {
			RPCCall<Derived>::create(this->rpc, this->service);
			auto ptr = this->getSharedPtr();
			this->launchInEventLoop([this, ptr](){
				ptr->impl(ptr, this->impl_detail.m_Request);
			});
			impl_detail.createWorker();
		}

		template<typename Derived, typename RPCType>
		template<typename X, boost::enable_if_t<std::is_same<X, BidiStream_t>::value>*>
		void RPCCall<Derived, RPCType>::handle(bool /*unused*/) {
			RPCCall<Derived>::create(this->rpc, this->service);
			auto ptr = this->getSharedPtr();
			this->launchInEventLoop([ptr](){
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
		auto weakContainer<C>::getLockedRange(const std::pair<It, It>& rng)
			-> Detail::range_type_it_t<
				decltype(locked_adapter()),
				decltype(locked_filter()), It> {
			auto range = boost::iterator_range<It>(rng.first, rng.second);
			auto locked = boost::adaptors::transform(range, locked_adapter());
			return boost::remove_if<boost::return_begin_found>(locked, locked_filter());
		}

		template<typename C>
		template<typename Idx>
		auto weakContainer<C>::getLockedIndex(const Idx&)
			-> Detail::range_type_idx_t<
				decltype(locked_adapter()),
				decltype(locked_filter()),
				decltype(mi::get<Idx>(container))
				> {
			std::lock_guard<decltype(m_Mtx)> lk(m_Mtx);
			const auto& idx = mi::get<Idx>(container);
			return getLockedRange(std::make_pair(idx.cbegin(), idx.cend()));
		}

		template<typename C>
		template<typename Functor>
		auto weakContainer<C>::getLocked(Functor&& func) -> Detail::range_type_func_t<decltype(locked_adapter()), decltype(locked_filter()), Functor, decltype(*this)> {
			std::lock_guard<decltype(m_Mtx)> lk(m_Mtx);
			const auto& range = func(*this);
			return this->getLockedRange(range);
		}
	}
}}

#include "MurmurRPC.proto.Wrapper.cpp"

#endif
#endif
