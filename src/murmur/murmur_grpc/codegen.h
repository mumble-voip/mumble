/// \file "codegen.h"
/// This just exists to show the various types of Derived objects that RPCCall can use.
///
/// All objects are deliberately *empty* having no member variables nor non-static
/// functions. This allows RPCCall to inherit them for *free* using via
/// [empty base optimization](https://en.cppreference.com/w/cpp/language/ebo).
///
/// The impl() and, if needed, onDone() methods for each struct are written in the end of MurmurGRPCImpl.cpp
///
namespace MurmurRPC {
namespace Wrapper {

/// \brief Model of a Unary service.grpc created by grpc codegen.
///
/// By setting the RPCType to Unary_t we let RPCCall know that we are a unary service. This
/// type of service take a single request message and sends either an error or a single
/// responce message. This by far the most common call type and the easiest to implement,
/// as you are only required to write one function.
///
struct GetUnaryFoo {

	/// \brief This will always be Detail::Unary_t in this service typel
	typedef Detail::Unary::rpctype RPCType;

	/// \brief incoming protobuf message type
	typedef FooSingleRequest InType;

	/// \brief outgoing protobuf message type
	typedef FooSingleResponce OutType;

	/// \brief Service this call is for. Always MurmurRPC::V1::AsyncService currently
	typedef MurmurRPC::V1::AsyncService Service;

	/// \brief gGRPC template to communicate with the service
	template<typename T>
		using StreamType = ::grpc::ServerAsyncResponseWriter<T>;

	/// \brief convenience typedef for the impl() function to use
	typedef const std::shared_ptr<RPCCall<GetUnaryFoo>>& rpcPtr;

	/// \brief Method that needs to be hand-written for this call
	///
	/// This method will always run in the main event loop thread.
	///
	/// With getting a std::shared_ptr to the RPCCall we live in, and
	/// the message of InType, it is expected to either call RPCCall::end()
	/// with an output message or throw a grpc::Status an as error.
	///
	/// If neither of these conditions are satisfied, it can hang the RPCCall
	/// until gRPC shuts it down due to a timeout condition. Likely this function
	/// signature will change i the future.
	///
	/// \param rpcPtr std::shared_ptr to the RPCCall we were derived from
	/// \param InType& protobuf request message
	static void impl(rpcPtr, InType&);

	/// \brief Gets the function pointer needed to add this call to the completion queue
	/// \return member function pointer of Service needed to hook into the completion queue
	static Service::*RequestGetUnaryFoo getRequestFn();
};

/// \brief Model of a Server Stream service created by grpc codegen.
///
/// By setting RPCType to ServerStream_t we let RPCCall know we are a server streaming service. This
/// type of service takes a single incoming request and then sends many messages to the client. These
/// could be Meta events like servers starting or stopping, or even events such as a user
/// joining or leaving a channel.
///
struct GetBazEvents {

	/// \brief always ServerStream_t for kind of call
	typedef Detail::ServerStream::rpctype RPCType;

	/// \brief Incoming protobuf request type.
	///
	/// Usually this defines any filters on the events to be listened for
	///
	typedef GetBazEventsRequest InType;

	/// \brief Outgoing protobuf request type.
	///
	/// These are typically created by slot listeners in MurmurGRPCImpl and sent to the client
	///
	typedef BazEvent OutType;

	/// \brief Service this call is for. Always MurmurRPC::V1::AsyncService currently
	typedef MurmurRPC::V1::AsyncService Service;

	/// \brief convenience typedef for the std::shared_ptr to our holding RPCCall
	typedef const std::shared_ptr<RPCCall<GetBazEvents>>& rpcPtr;

	/// \brief gGRPC template to communicate with the service
	template<typename T>
		using StreamType = grpc::ServerAsyncWriter<T>;

	/// \brief User-written method called when new request is recieved.
	///
	/// This method will always run in the main event loop thread.
	///
	/// A typical implementation of this method will check the validity of the
	/// request, then add an rpcId, std::weak_ptr and any other information needed
	/// to define what sort of event is to be listened for into a weakContainer.
	///
	/// This weakContainer is then used by the listeners in MurmurGRPCImpl to
	/// look up any service that needs to have a message sent to them when
	/// a matching event is recieved.
	///
	/// \param rpcPtr std::shared_ptr to the RPCCall object we were derived from
	/// \param InType& the incoming request to start a listener stream
	///
	static void impl(rpcPtr, InType&);

	/// \brief User-written method to be called when a listener is stopped.
	///
	/// This can can either be because the user canceled the call, or because an
	/// error occured. In theory, there could be a graceful way to stop listening
	/// but no service currently supports such an action.
	///
	/// A typical implementation just deletes the items from the weakContainer
	/// that held listeners by using the index on rpcId.
	///
	/// \param rpcPtr std::shared_ptr to the RPCCall we live in
	/// \param bool passed on from the completion queue callback. Ignored
	/// by most implementations.
	///
	static void onDone(rpcPtr, bool);

	/// \brief Gets the function pointer needed to add this call to the completion queue
	/// \return member function pointer of Service needed to hook into the completion queue
	static Service::*RequestGetBazEvents getRequestFn();
};

/// \brief Model of a BidiStream call generated by the grpc codegen.
///
/// By setting RPCType to BidiStream_t we let RPCCall know we are a streaming-streaming service. This
/// type of service takes a stream of incoming messages, and returns a stream ouf outgoing messages.
///
/// This call is the least common, and the hardest to use as the code in MurmurGRPCImpl has to be
/// far more aware of the gRPC interface. RPCCalls of this type provide nothing but methods
/// to read and write protobuf messages without blocking any thread by exposing the boost::fibers
/// details that the other types hide. They are also the most powerful types, with no limitations
/// on message ordering.
///
struct BarFilter {

	/// \brief always Detail::BidiStream_t for this type.
	typedef Detail::BidiStream::rpctype RPCType;

	/// \brief Incoming message type.
	///
	/// Current design is a bit off, as the initating request to start the stream
	/// is of the same type as the responses that wanted when an outgoing message is
	/// sent
	///
	/// Most current types use these as 'filters' of some sort such that the incoming
	/// message type, except on initation, is actually a reply to a request of some sort.
	///
	typedef BarFilterReply InType;

	/// \brief Outgoing message type.
	///
	/// These are usually requests generated from MurmurGRPCImpl to get a reply on some sort of
	/// action generated. This isn't mandated however as a Bidi Stream RPCCall can use any
	/// message order it wants---even sending multiple incoming messages before reading a reply
	/// or reading multiple messages before sending a response.
	///
	typedef BarFilterRequest OutType;

	/// \brief Service this call is for. Always MurmurRPC::V1::AsyncService currently
	typedef MurmurRPC::V1::AsyncService Service;

	/// \brief Convenience typedef for a std::shared_ptr reference to our RPCCall type
	typedef const std::shared_ptr<RPCCall<BarFilter>>& rpcPtr;

	/// \brief template alias to create the gRPC communication object
	template<typename T, typename U>
		using StreamType = ::grpc::ServerAsyncReaderWriter<T, U>;

	/// \brief User-defined function to be called on initation of new call.
	///
	/// Very similar to GetBazEvents::impl(), except there is no pre-read message. Only
	/// a std::shared_ptr to the RPC object is provided.
	///
	/// A typical implementation will then read in a message, check validity, and
	/// then add an rpcId and std::weak_ptr to the RPCCall, along with any other
	/// information needed to a weakContainer.
	///
	static void impl(rpcPtr);

	/// \brief User-written method to be called when a stream is stopped.
	///
	/// This can can either be because the user canceled the call, or because an
	/// error occured. In theory, there could be a graceful way to stop stream
	/// but no service currently supports such an action.
	///
	/// A typical implementation just deletes the items from the weakContainer
	/// that held listeners by using the index on rpcId.
	///
	/// \param rpcPtr std::shared_ptr to the RPCCall we live in
	/// \param bool passed on from the completion queue callback. Ignored
	/// by most implementations.
	///
	static void onDone(rpcPtr, bool);

	/// \brief Gets the function pointer needed to add this call to the completion queue
	/// \return member function pointer of Service needed to hook into the completion queue
	static Service::*RequestBarFilter getRequestFn();
};
}
}
