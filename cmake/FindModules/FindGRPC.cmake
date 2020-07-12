# https://github.com/IvanSafonov/grpc-cmake-example

# MIT License
#
# Copyright (c) 2018 Ivan Safonov
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#
# Locate and configure the gRPC library
#
# Adds the following targets:
#
#  gRPC::grpc - gRPC library
#  gRPC::grpc++ - gRPC C++ library
#  gRPC::grpc++_reflection - gRPC C++ reflection library
#  gRPC::grpc_cpp_plugin - C++ generator plugin for Protocol Buffers
#

# Find gRPC include directory
find_path(GRPC_INCLUDE_DIR grpc/grpc.h)
mark_as_advanced(GRPC_INCLUDE_DIR)

# Find gRPC library
if(NOT TARGET gRPC::grpc)
	find_library(GRPC_LIBRARY NAMES grpc)
	mark_as_advanced(GRPC_LIBRARY)
	add_library(gRPC::grpc UNKNOWN IMPORTED)
	set_target_properties(gRPC::grpc PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${GRPC_INCLUDE_DIR}
		INTERFACE_LINK_LIBRARIES "-lpthread;-ldl"
		IMPORTED_LOCATION ${GRPC_LIBRARY}
	)
endif()

# Find gRPC C++ library
if(NOT TARGET gRPC::grpc++)
	find_library(GRPC_GRPC++_LIBRARY NAMES grpc++)
	mark_as_advanced(GRPC_GRPC++_LIBRARY)
	add_library(gRPC::grpc++ UNKNOWN IMPORTED)
	set_target_properties(gRPC::grpc++ PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${GRPC_INCLUDE_DIR}
		INTERFACE_LINK_LIBRARIES gRPC::grpc
		IMPORTED_LOCATION ${GRPC_GRPC++_LIBRARY}
	)
endif()

# Find gRPC C++ reflection library
if(NOT TARGET gRPC::grpc++_reflection)
	find_library(GRPC_GRPC++_REFLECTION_LIBRARY NAMES grpc++_reflection)
	mark_as_advanced(GRPC_GRPC++_REFLECTION_LIBRARY)
	add_library(gRPC::grpc++_reflection UNKNOWN IMPORTED)
	set_target_properties(gRPC::grpc++_reflection PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${GRPC_INCLUDE_DIR}
		INTERFACE_LINK_LIBRARIES gRPC::grpc++
		IMPORTED_LOCATION ${GRPC_GRPC++_REFLECTION_LIBRARY}
	)
endif()

# Find gRPC CPP generator
if(NOT TARGET gRPC::grpc_cpp_plugin)
	find_program(GRPC_CPP_PLUGIN NAMES grpc_cpp_plugin)
	mark_as_advanced(GRPC_CPP_PLUGIN)
	add_executable(gRPC::grpc_cpp_plugin IMPORTED)
	set_target_properties(gRPC::grpc_cpp_plugin PROPERTIES
		IMPORTED_LOCATION ${GRPC_CPP_PLUGIN}
	)
endif()

if(TARGET gRPC::grpc AND TARGET gRPC::grpc++ AND TARGET gRPC::grpc++_reflection AND TARGET gRPC::grpc_cpp_plugin)
	set(GRPC_FOUND PARENT_SCOPE TRUE)
endif()
