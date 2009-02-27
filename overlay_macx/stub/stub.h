/*******************************************************************************
    stub.h - Overlay stub for Mumble

    Based on mach_inject_bundle_stub.c
        Copyright (c) 2005 Jonathan 'Wolf' Rentzsch: <http://rentzsch.com>
        Some rights reserved: <http://creativecommons.org/licenses/by/2.0/>
    
    Design inspired by SCPatchLoader, by Jon Gotow of St. Clair Software:
        <http://www.stclairsoft.com>

    ***************************************************************************/

#ifndef __STUB_H__
#define __STUB_H__

struct _StubParameters {
	ptrdiff_t offset;
	char      filename;
};

typedef struct _StubParameters StubParameters;

#endif /* __STUB_H__ */
