// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PROCESS_RESOLVER_H_
#define MUMBLE_PROCESS_RESOLVER_H_

#include <QtCore/QVector>

#include <cstdint>
#include <memory>
#include <unordered_map>

/// This ProcessResolver can be used to get a QVector of running process names and associated PIDs on multiple
/// platforms. This object is by no means thread-safe!
class ProcessResolver {
public:
	using ProcessMap = std::unordered_map< uint64_t, std::unique_ptr< char[] > >;

	/// @param resolveImmediately Whether the constructor should directly invoke ProcesResolver::resolve()
	ProcessResolver(bool resolveImmediately = true);
	virtual ~ProcessResolver();

	/// Resolves the namaes and PIDs of the running processes
	void resolve();
	/// @returns The ProcessMap holding the mapping between PID and process name of all processes
	/// found by this resolver
	const ProcessMap &getProcessMap() const;
	/// @returns The amount of processes that have been resolved by this object
	size_t amountOfProcesses() const;

protected:
	/// A map containing the PID->name mapping for the found processes
	ProcessMap m_processMap;

	/// The OS specific implementation of filling in details about running process names and PIDs
	void doResolve();
};

#endif // MUMBLE_PROCESS_RESOLVER_H_
