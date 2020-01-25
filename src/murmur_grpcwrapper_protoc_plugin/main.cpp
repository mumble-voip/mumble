// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <map>
#include <string>
#include <sstream>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

#include <boost/algorithm/string/replace.hpp>

using namespace std;

using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace google::protobuf::io;

const char *CSingle_SSingle = R"(
struct $service$_$method$ {
	typedef Detail::Unary::rpctype RPCType;
	typedef boost::remove_reference_t<decltype(std::declval<::$in$>())> InType;
	typedef boost::remove_reference_t<decltype(std::declval<::$out$>())> OutType;
	typedef boost::remove_reference_t<decltype(std::declval<::$ns$::$service$::AsyncService>())> Service;
	template<typename T>
		using StreamType = ::grpc::ServerAsyncResponseWriter<T>;
	typedef const std::shared_ptr<RPCCall<$service$_$method$>>& rpcPtr;

	static void impl(rpcPtr, InType&);

	static auto getRequestFn() -> decltype((&::$ns$::$service$::AsyncService::Request$method$)) {
		return &::$ns$::$service$::AsyncService::Request$method$;
	}
};
)";

const char *CSingle_SStream = R"(
struct $service$_$method$ {
	typedef Detail::ServerStream::rpctype RPCType;
	typedef boost::remove_reference_t<decltype(std::declval<::$in$>())> InType;
	typedef boost::remove_reference_t<decltype(std::declval<::$out$>())> OutType;
	typedef boost::remove_reference_t<decltype(std::declval<::$ns$::$service$::AsyncService>())> Service;
	typedef const std::shared_ptr<RPCCall<$service$_$method$>>& rpcPtr;

	template<typename T>
		using StreamType = ::grpc::ServerAsyncWriter<T>;

	static void impl(rpcPtr, InType&);
	static void onDone(rpcPtr, bool);

	static auto getRequestFn() -> decltype((&::$ns$::$service$::AsyncService::Request$method$)) {
		return &::$ns$::$service$::AsyncService::Request$method$;
	}
};
)";

const char *CStream_SSingle = R"(
struct $service$_$method$ {
	typedef Detail::ClientStream::rpctype RPCType;
	typedef boost::remove_reference_t<decltype(std::declval<::$in$>())> InType;
	typedef boost::remove_reference_t<decltype(std::declval<::$out$>())> OutType;
	typedef boost::remove_reference_t<decltype(std::declval<::$ns$::$service$::AsyncService>())> Service;
	typedef const std::shared_ptr<RPCCall<$service$_$method$>>& rpcPtr;

	static auto getRequestFn() -> decltype((&::$ns$::$service$::AsyncService::Request$method$)) {
		return &::$ns$::$service$::AsyncService::Request$method$;
	}
};
)";

const char *CStream_SStream = R"(
struct $service$_$method$ {
	typedef Detail::BidiStream::rpctype RPCType;
	typedef boost::remove_reference_t<decltype(std::declval<::$in$>())> InType;
	typedef boost::remove_reference_t<decltype(std::declval<::$out$>())> OutType;
	typedef boost::remove_reference_t<decltype(std::declval<::$ns$::$service$::AsyncService>())> Service;
	typedef const std::shared_ptr<RPCCall<$service$_$method$>>& rpcPtr;
	template<typename T, typename U>
		using StreamType = ::grpc::ServerAsyncReaderWriter<T, U>;
	static void impl(rpcPtr);
	static void onDone(rpcPtr, bool);

	static auto getRequestFn() -> decltype((&::$ns$::$service$::AsyncService::Request$method$)) {
		return &::$ns$::$service$::AsyncService::Request$method$;
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

		cpp.Print("#ifndef MUMBLE_MURMUR_MURUR_MURMURGRPCWRAPPER_H_\n");
		cpp.Print("#define MUMBLE_MURMUR_MURUR_MURMURGRPCWRAPPER_H_\n");

		map<string, string> tpl;
		tpl["ns"] = ns;

		for (int i = 0; i < input->service_count(); i++) {
			auto service = input->service(i);
			tpl["service"] = service->name();

			for (int j = 0; j < service->method_count(); j++) {
				auto method = service->method(j);
				tpl["method"] = method->name();
				tpl["in"] = CompiledName(ns, method->input_type());
				tpl["out"] = CompiledName(ns, method->output_type());

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

			cpp.Print("#endif\n");
			cpp.Print("#ifdef MUMBLE_MURMUR_GRPC_WRAPPER_IMPL\n");
			cpp.Print(tpl, "void $service$_Init(MurmurRPCImpl *impl, ::$ns$::$service$::AsyncService *service) {\n");
			for (int j = 0; j < service->method_count(); j++) {
				auto method = service->method(j);
				tpl["method"] = method->name();
				cpp.Print(tpl, "\tRPCCall<$service$_$method$>::create(impl, service);\n");
			}
			cpp.Print("}\n");
			cpp.Print("#endif\n");

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
