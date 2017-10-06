#include "stdafx.h"
#include "File.hpp"
#include "MgsFunction.hpp"

// Memory allocation
MGS_STDLIB(malloc, 0x00539990);
MGS_STDLIB(free, 0x0053A400);
MGS_STDLIB(realloc, 0x00539E20);
MGS_STDLIB(calloc, 0x00539DA0);

// File I/O
MGS_STDLIB(fopen, 0x0053CB40);
MGS_STDLIB(fputs, 0x0053C970);
MGS_STDLIB(fflush, 0x0053C6C0);
MGS_STDLIB(fclose, 0x0053C4A0);
MGS_STDLIB(open, 0x0053C5F0);
MGS_STDLIB(fprintf, 0x0053DBE0);
MGS_STDLIB(close, 0x0053D680);
MGS_STDLIB(lseek, 0x0053E180);
MGS_STDLIB(read, 0x0053D1A0);
