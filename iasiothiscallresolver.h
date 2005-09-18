/************************************************
 * This file is NOT covered by the Mumble License
 * It is an adapted version of IASIOThiscallResolver.h/.cpp, which
 * can be found at http://www.audiomulch.com/~rossb/code/calliasio
 *
 * Note that this file depends on the ASIO SDK, which is covered
 * by it's own license, and which you need to download separately.
 *
 * Original Authors: Fraser Adams
 *                   Ross Bencina
 *                   Rene G. Ceballos
 *                   Martin Fay
 *                   Antti Silvast
 *                   Andrew Baldwin
 *
 * This version is intended to use just as a wrapper for IASIO, it will
 * not fix up the theAsioDriver in the SDK. In addition, it attempts
 * to forward the IUnknown interface functions to the original IASIO, as
 * we need to support Release().
 *
 * It should also be copiable.
 */

#ifndef _IASIOTHISCALLRESOLVER_H
#define _IASIOTHISCALLRESOLVER_H

#include <windows.h>
#include <asiodrvr.h> /* From ASIO SDK */

#define CALL_THISCALL_0( resultName, thisPtr, funcOffset )                  \
    __asm__ __volatile__ ("movl (%1), %%edx\n\t"                            \
                          "call *"#funcOffset"(%%edx)\n\t"                  \
                          :"=a"(resultName) /* Output Operands */           \
                          :"c"(thisPtr)     /* Input Operands */            \
                         );                                                 \


#define CALL_VOID_THISCALL_1( thisPtr, funcOffset, param1 )                 \
    __asm__ __volatile__ ("pushl %0\n\t"                                    \
                          "movl (%1), %%edx\n\t"                            \
                          "call *"#funcOffset"(%%edx)\n\t"                  \
                          :                 /* Output Operands */           \
                          :"r"(param1),     /* Input Operands */            \
                           "c"(thisPtr)                                     \
                         );                                                 \


#define CALL_THISCALL_1( resultName, thisPtr, funcOffset, param1 )          \
    __asm__ __volatile__ ("pushl %1\n\t"                                    \
                          "movl (%2), %%edx\n\t"                            \
                          "call *"#funcOffset"(%%edx)\n\t"                  \
                          :"=a"(resultName) /* Output Operands */           \
                          :"r"(param1),     /* Input Operands */            \
                           "c"(thisPtr)                                     \
                          );                                                \


#define CALL_THISCALL_1_DOUBLE( resultName, thisPtr, funcOffset, param1 )   \
    __asm__ __volatile__ ("pushl 4(%1)\n\t"                                 \
                          "pushl (%1)\n\t"                                  \
                          "movl (%2), %%edx\n\t"                            \
                          "call *"#funcOffset"(%%edx);\n\t"                 \
                          :"=a"(resultName) /* Output Operands */           \
                          :"a"(&param1),    /* Input Operands */            \
                           /* Note: Using "r" above instead of "a" fails */ \
                           /* when using GCC 3.3.3, and maybe later versions*/\
                           "c"(thisPtr)                                     \
                          );                                                \


#define CALL_THISCALL_2( resultName, thisPtr, funcOffset, param1, param2 )  \
    __asm__ __volatile__ ("pushl %1\n\t"                                    \
                          "pushl %2\n\t"                                    \
                          "movl (%3), %%edx\n\t"                            \
                          "call *"#funcOffset"(%%edx)\n\t"                  \
                          :"=a"(resultName) /* Output Operands */           \
                          :"r"(param2),     /* Input Operands */            \
                           "r"(param1),                                     \
                           "c"(thisPtr)                                     \
                          );                                                \


#define CALL_THISCALL_4( resultName, thisPtr, funcOffset, param1, param2, param3, param4 )\
    __asm__ __volatile__ ("pushl %1\n\t"                                    \
                          "pushl %2\n\t"                                    \
                          "pushl %3\n\t"                                    \
                          "pushl %4\n\t"                                    \
                          "movl (%5), %%edx\n\t"                            \
                          "call *"#funcOffset"(%%edx)\n\t"                  \
                          :"=a"(resultName) /* Output Operands */           \
                          :"r"(param4),     /* Input Operands  */           \
                           "r"(param3),                                     \
                           "r"(param2),                                     \
                           "r"(param1),                                     \
                           "c"(thisPtr)                                     \
                          );                                                \

class IASIOThiscallResolver : public IASIO {
private:
	IASIO *that_;
public:
	IASIOThiscallResolver(IASIO *that) {
		that_ = that;
	};

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv) {
		return that_->QueryInterface(riid, ppv);
	};
    virtual ULONG STDMETHODCALLTYPE AddRef() {
		return that_->AddRef();
	};
    virtual ULONG STDMETHODCALLTYPE Release() {
		return that_->Release();
	};

	virtual ASIOBool init(void *sysHandle) {
	    ASIOBool result;
	    CALL_THISCALL_1( result, that_, 12, sysHandle );
	    return result;
	};
	virtual void getDriverName(char *name) {
		    CALL_VOID_THISCALL_1( that_, 16, name );
	};
	virtual long getDriverVersion() {
	    ASIOBool result;
	    CALL_THISCALL_0( result, that_, 20 );
	    return result;
	};
	virtual void getErrorMessage(char *string) {
	     CALL_VOID_THISCALL_1( that_, 24, string );
	};
	virtual ASIOError start() {
	    ASIOBool result;
	    CALL_THISCALL_0( result, that_, 28 );
	    return result;
	};
	virtual ASIOError stop() {
	    ASIOBool result;
	    CALL_THISCALL_0( result, that_, 32 );
	    return result;
	};
	virtual ASIOError getChannels(long *numInputChannels, long *numOutputChannels) {
	    ASIOBool result;
	    CALL_THISCALL_2( result, that_, 36, numInputChannels, numOutputChannels );
	    return result;
	};
	virtual ASIOError getLatencies(long *inputLatency, long *outputLatency) {
	    ASIOBool result;
	    CALL_THISCALL_2( result, that_, 40, inputLatency, outputLatency );
	    return result;
	};
	virtual ASIOError getBufferSize(long *minSize, long *maxSize, long *preferredSize, long *granularity) {
		ASIOBool result;
		CALL_THISCALL_4( result, that_, 44, minSize, maxSize, preferredSize, granularity );
		return result;
	};
	virtual ASIOError canSampleRate(ASIOSampleRate sampleRate) {
		ASIOBool result;
		CALL_THISCALL_1_DOUBLE( result, that_, 48, sampleRate );
		return result;
	};
	virtual ASIOError getSampleRate(ASIOSampleRate *sampleRate) {
		ASIOBool result;
		CALL_THISCALL_1( result, that_, 52, sampleRate );
		return result;
	};
	virtual ASIOError setSampleRate(ASIOSampleRate sampleRate) {
		ASIOBool result;
		CALL_THISCALL_1_DOUBLE( result, that_, 56, sampleRate );
		return result;
	};
	virtual ASIOError getClockSources(ASIOClockSource *clocks, long *numSources) {
		ASIOBool result;
		CALL_THISCALL_2( result, that_, 60, clocks, numSources );
		return result;
	};
	virtual ASIOError setClockSource(long reference) {
		ASIOBool result;
		CALL_THISCALL_1( result, that_, 64, reference );
		return result;
	};
	virtual ASIOError getSamplePosition(ASIOSamples *sPos, ASIOTimeStamp *tStamp) {
		ASIOBool result;
		CALL_THISCALL_2( result, that_, 68, sPos, tStamp );
		return result;
	};
	virtual ASIOError getChannelInfo(ASIOChannelInfo *info) {
		ASIOBool result;
		CALL_THISCALL_1( result, that_, 72, info );
		return result;
	};
	virtual ASIOError createBuffers(ASIOBufferInfo *bufferInfos, long numChannels, long bufferSize, ASIOCallbacks *callbacks) {
		ASIOBool result;
		CALL_THISCALL_4( result, that_, 76, bufferInfos, numChannels, bufferSize, callbacks );
		return result;
	};
	virtual ASIOError disposeBuffers() {
		ASIOBool result;
		CALL_THISCALL_0( result, that_, 80 );
		return result;
	};
	virtual ASIOError controlPanel() {
		ASIOBool result;
		CALL_THISCALL_0( result, that_, 84 );
		return result;
	};
	virtual ASIOError future(long selector,void *opt) {
		ASIOBool result;
		CALL_THISCALL_2( result, that_, 88, selector, opt );
		return result;
	};
	virtual ASIOError outputReady() {
		ASIOBool result;
		CALL_THISCALL_0( result, that_, 92 );
		return result;
	};
};

#endif



