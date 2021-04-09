# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(generated-components)
include(FindPythonInterpreter)

find_python_interpreter(
	VERSION 3
	INTERPRETER_OUT_VAR PYTHON_INTERPRETER
	REQUIRED
)

# Let cmake generate the ProtoValidator{.h,.cpp} files
set(MUMBLE_PROTO_VALIDATOR_HEADER "${MUMBLE_GENERATED_HEADER_DIR}/ProtoValidator.h")
set(MUMBLE_PROTO_VALIDATOR_SOURCE "${MUMBLE_GENERATED_SOURCE_DIR}/ProtoValidator.cpp")
set(MUMBLE_PROTO_VALIDATOR_SCRIPT "${MUMBLE_SCRIPT_DIR}/generate-protobuf-validator.py")

set(MUMBLE_PROTO_FILE "${CMAKE_SOURCE_DIR}/src/Mumble.proto")

add_custom_command(
	OUTPUT "${MUMBLE_PROTO_VALIDATOR_HEADER}" "${MUMBLE_PROTO_VALIDATOR_SOURCE}"
	COMMAND
		"${PYTHON_INTERPRETER}" "${MUMBLE_PROTO_VALIDATOR_SCRIPT}"
		"-i" "${MUMBLE_PROTO_FILE}"
		"--out-header" "${MUMBLE_PROTO_VALIDATOR_HEADER}"
		"--out-source" "${MUMBLE_PROTO_VALIDATOR_SOURCE}"
	COMMENT "Generating ProtoValidator files..."
	DEPENDS "${MUMBLE_PROTO_VALIDATOR_SCRIPT}" "${MUMBLE_PROTO_FILE}"
)

add_custom_target(
	generate_protovalidator
	DEPENDS "${MUMBLE_PROTO_VALIDATOR_HEADER}" "${MUMBLE_PROTO_VALIDATOR_SOURCE}"
)
