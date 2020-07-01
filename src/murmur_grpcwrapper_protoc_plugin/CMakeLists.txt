# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(protoc-gen-murmur-grpcwrapper "main.cpp")

target_link_libraries(protoc-gen-murmur-grpcwrapper
	PRIVATE
		protobuf::libprotobuf
		protobuf::libprotoc
)
