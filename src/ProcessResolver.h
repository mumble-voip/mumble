// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PROCESS_RESOLVER_H_
#define MUMBLE_PROCESS_RESOLVER_H_

#include <QtCore/QVector>

#include <cstdint>

/// This ProcessResolver can be used to get a QVector of running process names and associated PIDs on multiple
/// platforms. This object is by no means thread-safe!
class ProcessResolver {
protected:
	/// The vector for the pointers to the process names
	QVector< const char * > m_processNames;
	/// The vector for the process PIDs
	QVector< uint64_t > m_processPIDs;

	/// Deletes all names currently stored in processNames and clears processNames and processPIDs
	void freeAndClearData();
	/// The OS specific implementation of filling in details about running process names and PIDs
	void doResolve();

public:
	/// @param resolveImmediately Whether the constructor should directly invoke ProcesResolver::resolve()
	ProcessResolver(bool resolveImmediately = true);
	virtual ~ProcessResolver();

	/// Resolves the namaes and PIDs of the running processes
	void resolve();
	/// Gets a reference to the stored process names
	const QVector< const char * > &getProcessNames() const;
	/// Gets a reference to the stored process PIDs (corresponding to the names returned by
	/// ProcessResolver::getProcessNames())
	const QVector< uint64_t > &getProcessPIDs() const;
	/// @returns The amount of processes that have been resolved by this object
	size_t amountOfProcesses() const;
};

#endif // MUMBLE_PROCESS_RESOLVER_H_
