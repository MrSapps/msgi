#pragma once

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include "MgsFunction.hpp"

// We must call MSG version of stdlib functions for shared var, e.g the FILE* struct for the
// stdlib used by MSGI.exe isn't the same as ours, mixing them will lead to a bad time.
// Additionally we need to redirect the mgs_stdlibfunc() to stdlibfunc() when running as our own exe
// otherwise it will be a stub that does nothing.

// Warning 4996 is disabled globally for this library since we need to use the old names
// until all functions using stdlib are reimplemented.


// Memory allocation
EXTERN_MGS_STDLIB(malloc, 0x00539990);
EXTERN_MGS_STDLIB(free, 0x0053A400);
EXTERN_MGS_STDLIB(realloc, 0x00539E20);
EXTERN_MGS_STDLIB(calloc, 0x00539DA0);

// File I/O
EXTERN_MGS_STDLIB(fopen, 0x0053CB40);
EXTERN_MGS_STDLIB(fputs, 0x0053C970);
EXTERN_MGS_STDLIB(fflush, 0x0053C6C0);
EXTERN_MGS_STDLIB(fclose, 0x0053C4A0);

EXTERN_MGS_STDLIB(open, 0x0053C5F0);
EXTERN_MGS_STDLIB(fprintf, 0x0053DBE0);

EXTERN_MGS_STDLIB(close, 0x0053D680);
EXTERN_MGS_STDLIB(lseek, 0x0053E180);
EXTERN_MGS_STDLIB(read, 0x0053D1A0);
