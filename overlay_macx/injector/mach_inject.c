 /*******************************************************************************
	mach_inject.c
		Copyright (c) 2003-2005 Jonathan 'Wolf' Rentzsch: <http://rentzsch.com>
		Some rights reserved: <http://creativecommons.org/licenses/by/2.0/>

	***************************************************************************/

#include	"mach_inject.h"

#include <mach-o/dyld.h>
#include <mach-o/getsect.h>
#include <mach/mach.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <assert.h>
#include <stdlib.h> // for malloc()
#include <stdio.h>  // for printf()
#include <mach-o/fat.h> // for fat structure decoding
#include <mach-o/arch.h> // to know which is local arch
#include <fcntl.h> // for open/close
// for mmap()
#include <sys/types.h>
#include <sys/mman.h>

#ifndef	COMPILE_TIME_ASSERT( exp )
	#define COMPILE_TIME_ASSERT( exp ) { switch (0) { case 0: case (exp):; } }
#endif
#define ASSERT_CAST( CAST_TO, CAST_FROM ) \
	COMPILE_TIME_ASSERT( sizeof(CAST_TO)==sizeof(CAST_FROM) )

#if defined(__i386__)
void* fixedUpImageFromImage (
		const void *image, 
		unsigned long imageSize, 
		unsigned int jumpTableOffset, 
		unsigned int jumpTableSize,
		ptrdiff_t fixUpOffset);
#endif /* __i386__ */

/*******************************************************************************
*	
*	Interface
*	
*******************************************************************************/
#pragma mark	-
#pragma mark	(Interface)

	mach_error_t
mach_inject(
		const mach_inject_entry	threadEntry,
		const void				*paramBlock,
		size_t					paramSize,
		pid_t					targetProcess,
		vm_size_t				stackSize )
{
	assert( threadEntry );
	assert( targetProcess > 0 );
	assert( stackSize == 0 || stackSize > 1024 );
	
	//	Find the image.
	const void		*image;
	unsigned long	imageSize;
	unsigned int	jumpTableOffset;
	unsigned int	jumpTableSize;
	mach_error_t	err = machImageForPointer( threadEntry, &image, &imageSize, &jumpTableOffset, &jumpTableSize );

	//	Initialize stackSize to default if requested.
	if( stackSize == 0 )
		/** @bug
			We only want an 8K default, fix the plop-in-the-middle code below.
		*/
		stackSize = 16 * 1024;
	
	//	Convert PID to Mach Task ref.
	mach_port_t	remoteTask = 0;
	if( !err ) {
		err = task_for_pid( mach_task_self(), targetProcess, &remoteTask );
#if defined(__i386__)
		if (err == 5) fprintf(stderr, "Could not access task for pid %d. You probably need to add user to procmod group\n", targetProcess);
#endif
	}
	
	/** @todo
		Would be nice to just allocate one block for both the remote stack
		*and* the remoteCode (including the parameter data block once that's
		written.
	*/
	
	//	Allocate the remoteStack.
	vm_address_t remoteStack = (vm_address_t)NULL;
	if( !err )
		err = vm_allocate( remoteTask, &remoteStack, stackSize, 1 );
	
	//	Allocate the code.
	vm_address_t remoteCode = (vm_address_t)NULL;
	if( !err )
		err = vm_allocate( remoteTask, &remoteCode, imageSize, 1 );
	if( !err ) {
		ASSERT_CAST( pointer_t, image );
#if defined (__ppc__) || defined (__ppc64__)
		err = vm_write( remoteTask, remoteCode, (pointer_t) image, imageSize );
#elif defined (__i386__)
		// on intel, jump table use relative jump instructions (jmp), which means
		// the offset needs to be corrected. We thus copy the image and fix the offset by hand. 
		ptrdiff_t fixUpOffset = (ptrdiff_t) (image - remoteCode); 
		void * fixedUpImage = fixedUpImageFromImage(image, imageSize, jumpTableOffset, jumpTableSize, fixUpOffset);
		err = vm_write( remoteTask, remoteCode, (pointer_t) fixedUpImage, imageSize );
		free(fixedUpImage);
#endif
	}
	
	//	Allocate the paramBlock if specified.
	vm_address_t remoteParamBlock = (vm_address_t)NULL;
	if( !err && paramBlock != NULL && paramSize ) {
		err = vm_allocate( remoteTask, &remoteParamBlock, paramSize, 1 );
		if( !err ) {
			ASSERT_CAST( pointer_t, paramBlock );
			err = vm_write( remoteTask, remoteParamBlock,
					(pointer_t) paramBlock, paramSize );
		}
	}
	
	//	Calculate offsets.
	ptrdiff_t	threadEntryOffset, imageOffset;
	if( !err ) {
		//assert( (void*)threadEntry >= image && (void*)threadEntry <= (image+imageSize) );
		ASSERT_CAST( void*, threadEntry );
		threadEntryOffset = ((void*) threadEntry) - image;
		
		ASSERT_CAST( void*, remoteCode );
		imageOffset = ((void*) remoteCode) - image;
	}
	
	//	Allocate the thread.
	thread_act_t remoteThread;
#if defined (__ppc__) || defined (__ppc64__)
	if( !err ) {
		ppc_thread_state_t remoteThreadState;
		
		/** @bug
			Stack math should be more sophisticated than this (ala redzone).
		*/
		remoteStack += stackSize / 2;
		
		bzero( &remoteThreadState, sizeof(remoteThreadState) );
		
		ASSERT_CAST( unsigned int, remoteCode );
		remoteThreadState.srr0 = (unsigned int) remoteCode;
		remoteThreadState.srr0 += threadEntryOffset;
		assert( remoteThreadState.srr0 < (remoteCode + imageSize) );
		
		ASSERT_CAST( unsigned int, remoteStack );
		remoteThreadState.r1 = (unsigned int) remoteStack;
		
		ASSERT_CAST( unsigned int, imageOffset );
		remoteThreadState.r3 = (unsigned int) imageOffset;
		
		ASSERT_CAST( unsigned int, remoteParamBlock );
		remoteThreadState.r4 = (unsigned int) remoteParamBlock;
		
		ASSERT_CAST( unsigned int, paramSize );
		remoteThreadState.r5 = (unsigned int) paramSize;
		
		ASSERT_CAST( unsigned int, 0xDEADBEEF );
		remoteThreadState.lr = (unsigned int) 0xDEADBEEF;
		
#if 0
		printf( "remoteCode start: %p\n", (void*) remoteCode );
		printf( "remoteCode size: %ld\n", imageSize );
		printf( "remoteCode pc: %p\n", (void*) remoteThreadState.srr0 );
		printf( "remoteCode end: %p\n",
			(void*) (((char*)remoteCode)+imageSize) );
		fflush(0);
#endif
		
		err = thread_create_running( remoteTask, PPC_THREAD_STATE,
				(thread_state_t) &remoteThreadState, PPC_THREAD_STATE_COUNT,
				&remoteThread );
	}
#elif defined (__i386__)
	if( !err ) {
		
		i386_thread_state_t remoteThreadState;
		bzero( &remoteThreadState, sizeof(remoteThreadState) );
			
		vm_address_t dummy_thread_struct = remoteStack;
		remoteStack += (stackSize / 2); // this is the real stack
		// (*) increase the stack, since we're simulating a CALL instruction, which normally pushes return address on the stack
		remoteStack -= 4;
		
#define PARAM_COUNT 4
#define STACK_CONTENTS_SIZE ((1+PARAM_COUNT) * sizeof(unsigned int))
		unsigned int stackContents[1 + PARAM_COUNT]; // 1 for the return address and 1 for each param
		// first entry is return address (see above *)
		stackContents[0] = 0xDEADBEEF; // invalid return address.
		// then we push function parameters one by one.
		stackContents[1] =  imageOffset;
		stackContents[2] = remoteParamBlock;
		stackContents[3] = paramSize;
		// We use the remote stack we allocated as the fake thread struct. We should probably use a dedicated memory zone. 
		// We don't fill it with 0, vm_allocate did it for us
		stackContents[4] = dummy_thread_struct; 
		
		// push stackContents
		err = vm_write( remoteTask, remoteStack,
						(pointer_t) stackContents, STACK_CONTENTS_SIZE);

#if !__DARWIN_UNIX03
 #define __eip eip
 #define __esp esp
#endif

		// set remote Program Counter
		remoteThreadState.__eip = (unsigned int) (remoteCode);
		remoteThreadState.__eip += threadEntryOffset;  
		
		// set remote Stack Pointer
		ASSERT_CAST( unsigned int, remoteStack );
		remoteThreadState.__esp = (unsigned int) remoteStack;
		
		// create thread and launch it
		err = thread_create_running( remoteTask, i386_THREAD_STATE,
									 (thread_state_t) &remoteThreadState, i386_THREAD_STATE_COUNT,
									 &remoteThread );
	}
#else
#error architecture not supported
#endif
	
	if( err ) {
		if( remoteParamBlock )
			vm_deallocate( remoteTask, remoteParamBlock, paramSize );
		if( remoteCode )
			vm_deallocate( remoteTask, remoteCode, imageSize );
		if( remoteStack )
			vm_deallocate( remoteTask, remoteStack, stackSize );
	}
	
	return err;
}

	mach_error_t
machImageForPointer(
		const void *pointer,
		const void **image,
		unsigned long *size,
		unsigned int *jumpTableOffset,
		unsigned int *jumpTableSize )
{
	assert( pointer );
	assert( image );
	assert( size );
	
	unsigned long p = (unsigned long) pointer;
	
	if (jumpTableOffset && jumpTableSize) {
		*jumpTableOffset = 0;
		*jumpTableSize = 0;
	}
	
	unsigned long imageIndex, imageCount = _dyld_image_count();
	for( imageIndex = 0; imageIndex < imageCount; imageIndex++ ) {
		const struct mach_header *header = _dyld_get_image_header( imageIndex );
		const struct section *section = getsectbynamefromheader( header,
																	SEG_TEXT,
																	SECT_TEXT );
		long start = section->addr + _dyld_get_image_vmaddr_slide( imageIndex );
		long stop = start + section->size;
		if( p >= start && p <= stop ) {
			//	It is truely insane we have to stat() the file system in order
			//	to discover the size of an in-memory data structure.
			const char *imageName = _dyld_get_image_name( imageIndex );
			assert( imageName );
			struct stat sb;
			if( stat( imageName, &sb ) )
				return unix_err( errno );
			if( image ) {
				ASSERT_CAST( void*, header );
				*image = (void*) header;
			}
			if( size ) {
				;//assertUInt32( st_size );
				*size = sb.st_size;

				// needed for Universal binaries. Check if file is fat and get image size from there.
				int fd = open (imageName, O_RDONLY);

				if (fd == -1) {
					fprintf(stderr, "Unable to open() image. Error: %s\n", strerror(errno));
					exit(1);
				}

				size_t mapSize = *size;
				fprintf(stderr, "mapSize = %u\n", mapSize);
				char * fileImage = mmap (NULL, mapSize, PROT_READ, MAP_FILE|MAP_SHARED, fd, 0);

				if (fileImage == -1) {
					fprintf(stderr, "Unable to mmap() image. Error: %s\n", strerror(errno));
					exit(1);
				}

				struct fat_header* fatHeader = (struct fat_header *)fileImage;
				if (fatHeader->magic == OSSwapBigToHostInt32(FAT_MAGIC)) {
					//printf("This is a fat binary\n");
					uint32_t archCount = OSSwapBigToHostInt32(fatHeader->nfat_arch);
					
					NXArchInfo const *localArchInfo = NXGetLocalArchInfo();
					
					struct fat_arch* arch = (struct fat_arch *)(fileImage + sizeof(struct fat_header));
					struct fat_arch* matchingArch = NULL;
					
					int archIndex = 0;
					for (archIndex = 0; archIndex < archCount; archIndex++) {
						cpu_type_t cpuType = OSSwapBigToHostInt32(arch[archIndex].cputype);
						cpu_subtype_t cpuSubtype = OSSwapBigToHostInt32(arch[archIndex].cpusubtype);
						
						if (localArchInfo->cputype == cpuType) {
							matchingArch = arch + archIndex;
							if (localArchInfo->cpusubtype == cpuSubtype) break;
						}
					}
					
					if (matchingArch) {
						*size = OSSwapBigToHostInt32(matchingArch->size);
						//printf ("found arch-specific size : %p\n", *size);
					}
				}
				
				munmap (fileImage, mapSize);
				close (fd);
			}
			if (jumpTableOffset && jumpTableSize) {
				const struct section * jumpTableSection = getsectbynamefromheader( header,
																				   "__IMPORT",
																				   "__jump_table" );
				if (jumpTableSection) {
					*jumpTableOffset = jumpTableSection->offset;
					*jumpTableSize = jumpTableSection->size;
				}
			}
			return err_none;
		}
	}
	
	return err_threadEntry_image_not_found;
}

#if defined(__i386__)
void* fixedUpImageFromImage (
		const void *image, 
		unsigned long imageSize, 
		unsigned int jumpTableOffset, 
		unsigned int jumpTableSize, 
		ptrdiff_t fixUpOffset)
{
	// first copy the full image
	void *fixedUpImage = (void *) malloc ((size_t)imageSize);
	bcopy(image, fixedUpImage, imageSize);
	
	// address of jump table in copied image
	void *jumpTable = fixedUpImage + jumpTableOffset;
	// each JMP instruction is 5 bytes (E9 xx xx xx xx) where E9 is the opcode for JMP
	int jumpTableCount = jumpTableSize / 5;
	
	// skip first "E9"
	jumpTable++;
	
	int entry=0;
	for (entry = 0; entry < jumpTableCount; entry++) {
		unsigned int jmpValue = *((unsigned int *)jumpTable);
		jmpValue += fixUpOffset;
		*((unsigned int *)jumpTable) = jmpValue;
		jumpTable+=5;
	}
	
	return fixedUpImage;
}
#endif /* __i386__ */
