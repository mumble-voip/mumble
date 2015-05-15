/* Copyright (C) 2015, Mikkel Krautz <mikkel@krautz.dk>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MUMBLE_OVERLAY_EXE_H_
#define MUMBLE_OVERLAY_EXE_H_

#if defined(_M_IX86)
const wchar_t *MUMBLE_OVERLAY_DLL_NAME = L"mumble_ol.dll";
#elif defined (_M_X64)
const wchar_t *MUMBLE_OVERLAY_DLL_NAME = L"mumble_ol_x64.dll";
#endif

/// OverlayHelperError represents exit codes returned by the
/// overlay helper process (mumble_ol_helper.exe) on Windows.
enum OverlayHelperError {
	/// The overlay helper process was not passed any
	/// arguments.
	OVERLAY_HELPER_ERROR_EXE_NO_ARGUMENTS            = 0x0F000001,
	/// The magic number on the command line of the overlay
	/// helper process could not be converted to an integer.
	OVERLAY_HELPER_ERROR_EXE_INVALID_MAGIC_ARGUMENT  = 0x0F000002,
	/// The magic number on the command line of the overlay
	/// helper did not match the built-in magic number of
	/// the helper process.
	OVERLAY_HELPER_ERROR_EXE_MAGIC_MISMATCH          = 0x0F000003,
	/// The overlay helper process was unable to configure
	/// its environment in preparation of loading the
	/// overlay DLL.
	OVERLAY_HELPER_ERROR_EXE_CONFIGURE_ENVIRONMENT   = 0x0F000004,
	/// The overlay helper process was unable to get the
	/// path to the overlay DLL.
	OVERLAY_HELPER_ERROR_EXE_GET_DLL_PATH            = 0x0F000005,
	/// The overlay helper process was unable to load the
	/// overlay DLL.
	OVERLAY_HELPER_ERROR_EXE_LOAD_DLL                = 0x0F000006,
	/// The overlay helper process was uanble to look up
	/// the 'OverlayHelperProcessMain' entry point in the
	/// overlay DLL.
	OVERLAY_HELPER_ERROR_EXE_LOOKUP_ENTRY_POINT      = 0x0F000007,
	/// The overlay helper process was uanble to parse
	/// the commandline arguments it was passed.
	/// The helper process exepcts two arguments, and this
	/// error occurs if it only finds one.
	OVERLAY_HELPER_ERROR_TOO_FEW_ARGUMENTS           = 0x0F000008,
	/// The magic number on the command line of the overlay
	/// helper process could not be converted to a HANDLE.
	OVERLAY_HELPER_ERROR_EXE_INVALID_HANDLE_ARGUMENT = 0x0F000009,

	/// The magic number passed to the overlay DLL's
	/// OverlayHelperProcessMain function did not match
	/// the overlay DLL's built-in magic number.
	OVERLAY_HELPER_ERROR_DLL_MAGIC_MISMATCH          = 0x0FF00000,
	/// The overlay helper process exited due to an error
	/// in the Windows message loop.
	OVERLAY_HELPER_ERROR_DLL_MESSAGE_LOOP            = 0x0FF00001,
	/// The parent death thread could not be created.
	OVERLAY_HELPER_ERROR_DLL_PDEATH_THREAD_ERROR     = 0x0FF00002,
	/// The helper's WaitForSingleObject call on its parent
	/// process failed unexpectedly.
	OVERLAY_HELPER_ERROR_DLL_PDEATH_WAIT_FAIL        = 0x0FF00003,
};

/// OverlayHelperErrorToString converts an OverlayHelperError value
/// to a printable string representation.
static inline const char *OverlayHelperErrorToString(OverlayHelperError err) {
	#define OHE(x) case x: return #x
	switch (err) {
		OHE(OVERLAY_HELPER_ERROR_EXE_NO_ARGUMENTS);
		OHE(OVERLAY_HELPER_ERROR_EXE_INVALID_MAGIC_ARGUMENT);
		OHE(OVERLAY_HELPER_ERROR_EXE_MAGIC_MISMATCH);
		OHE(OVERLAY_HELPER_ERROR_EXE_CONFIGURE_ENVIRONMENT);
		OHE(OVERLAY_HELPER_ERROR_EXE_GET_DLL_PATH);
		OHE(OVERLAY_HELPER_ERROR_EXE_LOAD_DLL);
		OHE(OVERLAY_HELPER_ERROR_EXE_LOOKUP_ENTRY_POINT);
		OHE(OVERLAY_HELPER_ERROR_TOO_FEW_ARGUMENTS);
		OHE(OVERLAY_HELPER_ERROR_EXE_INVALID_HANDLE_ARGUMENT);
		OHE(OVERLAY_HELPER_ERROR_DLL_MAGIC_MISMATCH);
		OHE(OVERLAY_HELPER_ERROR_DLL_MESSAGE_LOOP);
		OHE(OVERLAY_HELPER_ERROR_DLL_PDEATH_THREAD_ERROR);
		OHE(OVERLAY_HELPER_ERROR_DLL_PDEATH_WAIT_FAIL);
	}
	return NULL;
}

#endif
