// Copyright 2016-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <map>
#include <sstream>
#include <string>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/bind/bind.hpp>

using namespace std;

using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace google::protobuf::io;

const char *CSingle_SSingle = R"(
class $service$_$method$ : public RPCSingleSingleCall< ::$in$, ::$out$ > {
public:
	::$ns$::$service$::AsyncService *service;

	$service$_$method$(MurmurRPCImpl *rpc_impl, ::$ns$::$service$::AsyncService *async_service) : RPCSingleSingleCall(rpc_impl), service(async_service) {
	}

	void impl(bool ok);

	void handle(bool ok) {
		$service$_$method$::create(this->rpc, this->service);
		auto ie = new RPCExecEvent(::boost::bind(&$service$_$method$::impl, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}

	static void create(MurmurRPCImpl *rpc, ::$ns$::$service$::AsyncService *service) {
		auto call = new $service$_$method$(rpc, service);
		auto fn = ::boost::bind(&$service$_$method$::handle, call, boost::placeholders::_1);
		auto fn_ptr = new ::boost::function<void(bool)>(fn);
		service->Request$method$(&call->context, &call->request, &call->stream, rpc->m_completionQueue.get(), rpc->m_completionQueue.get(), fn_ptr);
	}
};
)";

const char *CSingle_SStream = R"(
class $service$_$method$ : public RPCSingleStreamCall< ::$in$, ::$out$ > {
public:
	::$ns$::$service$::AsyncService *service;

	$service$_$method$(MurmurRPCImpl *rpc_impl, ::$ns$::$service$::AsyncService *async_service) : RPCSingleStreamCall(rpc_impl), service(async_service) {
	}

	void impl(bool ok);
	void done(bool ok);

	::boost::function<void(bool)> *callback(::boost::function<void($service$_$method$ *, bool)> cb) {
		auto fn = ::boost::bind(&$service$_$method$::callbackAction, this, cb, boost::placeholders::_1);
		return new ::boost::function<void(bool)>(fn);
	}

	void handle(bool ok) {
		$service$_$method$::create(this->rpc, this->service);
		auto ie = new RPCExecEvent(::boost::bind(&$service$_$method$::impl, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}

	void handleDone(bool ok) {
		auto ie = new RPCExecEvent(::boost::bind(&$service$_$method$::done, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}

	static void create(MurmurRPCImpl *rpc, ::$ns$::$service$::AsyncService *service) {
		auto call = new $service$_$method$(rpc, service);
		auto done_fn = ::boost::bind(&$service$_$method$::handleDone, call, boost::placeholders::_1);
		auto done_fn_ptr = new ::boost::function<void(bool)>(done_fn);
		call->context.AsyncNotifyWhenDone(done_fn_ptr);
		auto fn = ::boost::bind(&$service$_$method$::handle, call, boost::placeholders::_1);
		auto fn_ptr = new ::boost::function<void(bool)>(fn);
		service->Request$method$(&call->context, &call->request, &call->stream, rpc->m_completionQueue.get(), rpc->m_completionQueue.get(), fn_ptr);
	}

private:

	void callbackAction(::boost::function<void($service$_$method$ *, bool)> cb, bool ok) {
		auto ie = new RPCExecEvent(::boost::bind(cb, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}
};
)";

const char *CStream_SSingle = R"(
class $service$_$method$ : public RPCCall {
public:
	::$ns$::$service$::AsyncService *service;

	::grpc::ServerAsyncReader< ::$out$, ::$in$ > stream;

	$service$_$method$(MurmurRPCImpl *rpc_impl, ::$ns$::$service$::AsyncService *async_service) : RPCCall(rpc_impl), service(async_service), stream(&context) {
	}

	void impl(bool ok);

	void error(const ::grpc::Status &err) {
		stream.FinishWithError(err, this->done());
	}

	void handle(bool ok) {
		$service$_$method$::create(this->rpc, this->service);
		auto ie = new RPCExecEvent(::boost::bind(&$service$_$method$::impl, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}

	static void create(MurmurRPCImpl *rpc, ::$ns$::$service$::AsyncService *service) {
		auto call = new $service$_$method$(rpc, service);
		auto fn = ::boost::bind(&$service$_$method$::handle, call, boost::placeholders::_1);
		auto fn_ptr = new ::boost::function<void(bool)>(fn);
		service->Request$method$(&call->context, &call->stream, rpc->m_completionQueue.get(), rpc->m_completionQueue.get(), fn_ptr);
	}
};
)";

const char *CStream_SStream = R"(
class $service$_$method$ : public RPCStreamStreamCall< ::$in$, ::$out$ > {
public:
	::$ns$::$service$::AsyncService *service;

	$service$_$method$(MurmurRPCImpl *rpc_impl, ::$ns$::$service$::AsyncService *async_service) : RPCStreamStreamCall(rpc_impl), service(async_service) {
	}

	void impl(bool ok);
	void done(bool ok);

	bool write() {
		bool processed = false;
		bool success;
		auto cb = [&success, &processed] ($service$_$method$ *, bool ok) {
			success = ok;
			processed = true;
		};
		stream.Write(response, callback(cb));
		while (!processed) {
			QCoreApplication::sendPostedEvents();
		}
		return success;
	}

	bool read() {
		bool processed = false;
		bool success;
		auto cb = [&success, &processed] ($service$_$method$ *, bool ok) {
			success = ok;
			processed = true;
		};
		stream.Read(&request, callback(cb));
		while (!processed) {
			QCoreApplication::sendPostedEvents();
		}
		return success;
	}

	bool writeRead() {
		return write() && read();
	}

	::boost::function<void(bool)> *callback(::boost::function<void($service$_$method$ *, bool)> cb) {
		auto fn = ::boost::bind(&$service$_$method$::callbackAction, this, cb, boost::placeholders::_1);
		return new ::boost::function<void(bool)>(fn);
	}

	void handle(bool ok) {
		$service$_$method$::create(this->rpc, this->service);
		auto ie = new RPCExecEvent(::boost::bind(&$service$_$method$::impl, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}

	void handleDone(bool ok) {
		auto ie = new RPCExecEvent(::boost::bind(&$service$_$method$::done, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}

	static void create(MurmurRPCImpl *rpc, ::$ns$::$service$::AsyncService *service) {
		auto call = new $service$_$method$(rpc, service);
		auto done_fn = ::boost::bind(&$service$_$method$::handleDone, call, boost::placeholders::_1);
		auto done_fn_ptr = new ::boost::function<void(bool)>(done_fn);
		call->context.AsyncNotifyWhenDone(done_fn_ptr);
		auto fn = ::boost::bind(&$service$_$method$::handle, call, boost::placeholders::_1);
		auto fn_ptr = new ::boost::function<void(bool)>(fn);
		service->Request$method$(&call->context, &call->stream, rpc->m_completionQueue.get(), rpc->m_completionQueue.get(), fn_ptr);
	}

private:

	void callbackAction(::boost::function<void($service$_$method$ *, bool)> cb, bool ok) {
		auto ie = new RPCExecEvent(::boost::bind(cb, this, ok), this);
		QCoreApplication::instance()->postEvent(rpc, ie);
	}
};
)";

class WrapperGenerator : public CodeGenerator {
	bool Generate(const FileDescriptor *input, const string &, GeneratorContext *context, string *) const {
		auto cpp_source = context->Open(input->name() + ".Wrapper.cpp");
		Printer cpp(cpp_source, '$');

		cpp.Print("// DO NOT EDIT!\n");
		cpp.Print("// Auto generated by protoc-gen-murmur-grpcwrapper\n");
		cpp.Print("\n");

		auto ns = ConvertDot(input->package());

		int namespaces = 1;
		{
			stringstream stream(input->package());
			string current;
			while (getline(stream, current, '.')) {
				cpp.Print("namespace $current$ {\n", "current", current);
				namespaces++;
			}
		}
		cpp.Print("namespace Wrapper {\n", "ns", ns);

		map< string, string > tpl;
		tpl["ns"] = ns;

		for (int i = 0; i < input->service_count(); i++) {
			auto service   = input->service(i);
			tpl["service"] = service->name();

			for (int j = 0; j < service->method_count(); j++) {
				auto method   = service->method(j);
				tpl["method"] = method->name();
				tpl["in"]     = CompiledName(ns, method->input_type());
				tpl["out"]    = CompiledName(ns, method->output_type());

				const char *template_str;
				if (method->client_streaming()) {
					if (method->server_streaming()) {
						template_str = CStream_SStream;
					} else {
						template_str = CStream_SSingle;
					}
				} else {
					if (method->server_streaming()) {
						template_str = CSingle_SStream;
					} else {
						template_str = CSingle_SSingle;
					}
				}
				cpp.Print(tpl, template_str);
			}

			cpp.Print(tpl, "void $service$_Init(MurmurRPCImpl *impl, ::$ns$::$service$::AsyncService *service) {\n");
			for (int j = 0; j < service->method_count(); j++) {
				auto method   = service->method(j);
				tpl["method"] = method->name();
				cpp.Print(tpl, "\t$service$_$method$::create(impl, service);\n");
			}
			cpp.Print("}\n");
		}

		cpp.Print("\n");
		while (namespaces--) {
			cpp.Print("}\n");
		}

		return true;
	}

	static string CompiledName(const string &ns, const Descriptor *type) {
		string s = type->name();
		for (type = type->containing_type(); type; type = type->containing_type()) {
			s = type->name() + "_" + s;
		}
		return ns + "::" + s;
	}

	static string ConvertDot(const string &str, const string &replace = "::") {
		return boost::replace_all_copy(str, ".", replace);
	}
};

int main(int argc, char *argv[]) {
	WrapperGenerator generator;
	return PluginMain(argc, argv, &generator);
}
