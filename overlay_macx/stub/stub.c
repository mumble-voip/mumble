/*******************************************************************************
	stub.c - Overlay stub for Mumble

	Based on mach_inject_bundle_stub.c
		Copyright (c) 2003-2009 Jonathan 'Wolf' Rentzsch: <http://rentzsch.com>
		Some rights reserved: <http://opensource.org/licenses/mit-license.php>

	Design inspired by SCPatchLoader, by Jon Gotow of St. Clair Software:
		<http://www.stclairsoft.com>

	***************************************************************************/

#include "../injector/mach_inject.h" // for INJECT_ENTRY
#include "stub.h"

#include <stdio.h>
#include <assert.h>
#include <dlfcn.h>
#include <mach/mach_init.h>
#include <mach/thread_act.h>
#include <pthread.h>
#include <Carbon/Carbon.h>

void INJECT_ENTRY(ptrdiff_t offset, StubParameters *p, size_t plen, char *pthread_dummy);
void *pthread_entry(StubParameters *p);
pascal void EventLoopTimerEntry(EventLoopTimerRef inTimer, StubParameters *p);

void
INJECT_ENTRY(ptrdiff_t offset, StubParameters *p, size_t plen, char *pthread_dummy)
{	
	assert(p);
	p->offset = offset;

#if defined (__i386__)
	// On intel, per-pthread data is a zone of data that must be allocated.
	// if not, all function trying to access per-pthread data (all mig functions for instance)
	// will crash. 
	extern void __pthread_set_self(char*);
	__pthread_set_self(pthread_dummy);
#endif

	pthread_attr_t attr;
	pthread_attr_init(&attr); 

	int policy;
	pthread_attr_getschedpolicy( &attr, &policy );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
	pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );

	struct sched_param sched;
	sched.sched_priority = sched_get_priority_max( policy );
	pthread_attr_setschedparam( &attr, &sched );

	pthread_t thread;
	pthread_create( &thread,
					&attr,
					(void* (*)(void*))((long)pthread_entry + offset),
					(void*) p );
	pthread_attr_destroy(&attr);

	thread_suspend(mach_thread_self());
}

void *
pthread_entry(StubParameters *p)
{
	assert(p);

	EventLoopTimerProcPtr proc = (EventLoopTimerProcPtr) EventLoopTimerEntry;
	proc += p->offset;
	EventLoopTimerUPP upp = NewEventLoopTimerUPP( proc );

	InstallEventLoopTimer( GetMainEventLoop(), 0.0, 0.0, upp, (void*)p, NULL );
	return NULL;
}

pascal void
EventLoopTimerEntry(EventLoopTimerRef inTimer, StubParameters *p)
{
	void *handle = NULL;

	assert(inTimer);
	assert(p);

	handle = dlopen(&p->filename, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "Unable to load file `%s'. Bailing.\n", &p->filename);
		return;
	}
}
