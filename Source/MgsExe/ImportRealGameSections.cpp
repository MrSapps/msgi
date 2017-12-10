#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "MgsFunction.hpp"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
extern char gRealGameSectionsBuffer[0x5B139C];

void ConsoleInit()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    SetConsoleTitleA("Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

void ApplyDetours()
{
    LOG_INFO("Applying detours...");
    MgsFunctionBase::ApplyFunctions();
    LOG_INFO("Detours done");
}

struct ImportPointer
{
    DWORD mTarget;
    const char* mFuncName;
    const char* mDllName;
};

const ImportPointer gImports[] = 
{
    { 0x0064A000,         "RegCloseKey",                "ADVAPI32" },
    { 0x0064A004,         "RegOpenKeyExA",              "ADVAPI32" },
    { 0x0064A008,         "RegQueryValueExA",           "ADVAPI32" },
    { 0x0064A010,         "DirectDrawEnumerateExA",     "DDRAW" },
    { 0x0064A014,         "DirectDrawCreateEx",         "DDRAW" },
    { 0x0064A01C,         "DirectInputCreateEx",        "DINPUT" },
    { 0x0064A024,         "DirectSoundCreate",          "DSOUND" },
    { 0x0064A02C,         "SetTextColor",               "GDI32" },
    { 0x0064A030,         "BitBlt",                     "GDI32" },
    { 0x0064A034,         "SelectObject",               "GDI32" },
    { 0x0064A038,         "DeleteDC",                   "GDI32" },
    { 0x0064A03C,         "DeleteObject",               "GDI32" },
    { 0x0064A040,         "GetObjectA",                 "GDI32" },
    { 0x0064A044,         "CreateCompatibleDC",         "GDI32" },
    { 0x0064A048,         "GetTextExtentPoint32A",      "GDI32" },
    { 0x0064A04C,         "GetTextMetricsA",            "GDI32" },
    { 0x0064A050,         "CreateFontA",                "GDI32" },
    { 0x0064A054,         "Polyline",                   "GDI32" },
    { 0x0064A058,         "CreatePen",                  "GDI32" },
    { 0x0064A05C,         "SetMapMode",                 "GDI32" },
    { 0x0064A060,         "CreateSolidBrush",           "GDI32" },
    { 0x0064A064,         "RemoveFontResourceA",        "GDI32" },
    { 0x0064A068,         "Polygon",                    "GDI32" },
    { 0x0064A06C,         "SetMapperFlags",             "GDI32" },
    { 0x0064A070,         "AddFontResourceA",           "GDI32" },
    { 0x0064A074,         "GetDeviceCaps",              "GDI32" },
    { 0x0064A078,         "RoundRect",                  "GDI32" },
    { 0x0064A07C,         "StretchBlt",                 "GDI32" },
    { 0x0064A080,         "SetBkColor",                 "GDI32" },
    { 0x0064A084,         "GetStockObject",             "GDI32" },
    { 0x0064A088,         "SetTextAlign",               "GDI32" },
    { 0x0064A08C,         "CreateCompatibleBitmap",     "GDI32" },
    { 0x0064A090,         "SetStretchBltMode",          "GDI32" },
    { 0x0064A094,         "GetDIBits",                  "GDI32" },
    { 0x0064A098,         "GetTextAlign",               "GDI32" },
    { 0x0064A09C,         "Rectangle",                  "GDI32" },
    { 0x0064A0A0,         "SetBkMode",                  "GDI32" },
    { 0x0064A0A4,         "TextOutA",                   "GDI32" },
    { 0x0064A0AC,         "GetStartupInfoA",            "KERNEL32" },
    { 0x0064A0B0,         "CreateThread",               "KERNEL32" },
    { 0x0064A0B4,         "ExitProcess",                "KERNEL32" },
    { 0x0064A0B8,         "IsBadWritePtr",              "KERNEL32" },
    { 0x0064A0BC,         "GetModuleHandleA",           "KERNEL32" },
    { 0x0064A0C0,         "GetCommandLineA",            "KERNEL32" },
    { 0x0064A0C4,         "GetVersion",                 "KERNEL32" },
    { 0x0064A0C8,         "IsBadReadPtr",               "KERNEL32" },
    { 0x0064A0CC,         "GetLastError",               "KERNEL32" },
    { 0x0064A0D0,         "CreateDirectoryA",           "KERNEL32" },
    { 0x0064A0D4,         "RtlUnwind",                  "KERNEL32" },
    { 0x0064A0D8,         "GetSystemTimeAsFileTime",    "KERNEL32" },
    { 0x0064A0DC,         "DeleteFileA",                "KERNEL32" },
    { 0x0064A0E0,         "ReadFile",                   "KERNEL32" },
    { 0x0064A0E4,         "HeapValidate",               "KERNEL32" },
    { 0x0064A0E8,         "SetFileAttributesA",         "KERNEL32" },
    { 0x0064A0EC,         "GetFileInformationByHandle", "KERNEL32" },
    { 0x0064A0F0,         "FileTimeToSystemTime",       "KERNEL32" },
    { 0x0064A0F4,         "GetFileType",                "KERNEL32" },
    { 0x0064A0F8,         "CreateFileA",                "KERNEL32" },
    { 0x0064A0FC,         "SetFilePointer",             "KERNEL32" },
    { 0x0064A100,         "WriteFile",                  "KERNEL32" },
    { 0x0064A104,         "TerminateProcess",           "KERNEL32" },
    { 0x0064A108,         "InterlockedDecrement",       "KERNEL32" },
    { 0x0064A10C,         "InterlockedIncrement",       "KERNEL32" },
    { 0x0064A110,         "TlsSetValue",                "KERNEL32" },
    { 0x0064A114,         "ExitThread",                 "KERNEL32" },
    { 0x0064A118,         "UnhandledExceptionFilter",   "KERNEL32" },
    { 0x0064A11C,         "GetModuleFileNameA",         "KERNEL32" },
    { 0x0064A120,         "FreeEnvironmentStringsA",    "KERNEL32" },
    { 0x0064A124,         "FreeEnvironmentStringsW",    "KERNEL32" },
    { 0x0064A128,         "WideCharToMultiByte",        "KERNEL32" },
    { 0x0064A12C,         "FileTimeToLocalFileTime",    "KERNEL32" },
    { 0x0064A130,         "PeekNamedPipe",              "KERNEL32" },
    { 0x0064A134,         "SetHandleCount",             "KERNEL32" },
    { 0x0064A138,         "GetStdHandle",               "KERNEL32" },
    { 0x0064A13C,         "DeleteCriticalSection",      "KERNEL32" },
    { 0x0064A140,         "WaitForSingleObject",        "KERNEL32" },
    { 0x0064A144,         "GetFileAttributesA",         "KERNEL32" },
    { 0x0064A148,         "SetLastError",               "KERNEL32" },
    { 0x0064A14C,         "TlsGetValue",                "KERNEL32" },
    { 0x0064A150,         "GetCurrentThread",           "KERNEL32" },
    { 0x0064A154,         "HeapDestroy",                "KERNEL32" },
    { 0x0064A158,         "HeapCreate",                 "KERNEL32" },
    { 0x0064A15C,         "HeapFree",                   "KERNEL32" },
    { 0x0064A160,         "VirtualFree",                "KERNEL32" },
    { 0x0064A164,         "EnterCriticalSection",       "KERNEL32" },
    { 0x0064A168,         "LeaveCriticalSection",       "KERNEL32" },
    { 0x0064A16C,         "DebugBreak",                 "KERNEL32" },
    { 0x0064A170,         "InitializeCriticalSection",  "KERNEL32" },
    { 0x0064A174,         "FatalAppExitA",              "KERNEL32" },
    { 0x0064A178,         "HeapAlloc",                  "KERNEL32" },
    { 0x0064A17C,         "Sleep",                      "KERNEL32" },
    { 0x0064A180,         "VirtualAlloc",               "KERNEL32" },
    { 0x0064A184,         "FlushFileBuffers",           "KERNEL32" },
    { 0x0064A188,         "SetStdHandle",               "KERNEL32" },
    { 0x0064A18C,         "SetEndOfFile",               "KERNEL32" },
    { 0x0064A190,         "MultiByteToWideChar",        "KERNEL32" },
    { 0x0064A194,         "LCMapStringA",               "KERNEL32" },
    { 0x0064A198,         "LCMapStringW",               "KERNEL32" },
    { 0x0064A19C,         "GetCPInfo",                  "KERNEL32" },
    { 0x0064A1A0,         "GetACP",                     "KERNEL32" },
    { 0x0064A1A4,         "GetOEMCP",                   "KERNEL32" },
    { 0x0064A1A8,         "SetConsoleCtrlHandler",      "KERNEL32" },
    { 0x0064A1AC,         "GetStringTypeA",             "KERNEL32" },
    { 0x0064A1B0,         "GetStringTypeW",             "KERNEL32" },
    { 0x0064A1B4,         "GetTimeZoneInformation",     "KERNEL32" },
    { 0x0064A1B8,         "GetExitCodeProcess",         "KERNEL32" },
    { 0x0064A1BC,         "CreateProcessA",             "KERNEL32" },
    { 0x0064A1C0,         "IsValidLocale",              "KERNEL32" },
    { 0x0064A1C4,         "IsValidCodePage",            "KERNEL32" },
    { 0x0064A1C8,         "GetLocaleInfoA",             "KERNEL32" },
    { 0x0064A1CC,         "EnumSystemLocalesA",         "KERNEL32" },
    { 0x0064A1D0,         "GetUserDefaultLCID",         "KERNEL32" },
    { 0x0064A1D4,         "GetLocaleInfoW",             "KERNEL32" },
    { 0x0064A1D8,         "CompareStringA",             "KERNEL32" },
    { 0x0064A1DC,         "CompareStringW",             "KERNEL32" },
    { 0x0064A1E0,         "SetEnvironmentVariableA",    "KERNEL32" },
    { 0x0064A1E4,         "ResetEvent",                 "KERNEL32" },
    { 0x0064A1E8,         "TerminateThread",            "KERNEL32" },
    { 0x0064A1EC,         "GetCurrentThreadId",         "KERNEL32" },
    { 0x0064A1F0,         "SetEvent",                   "KERNEL32" },
    { 0x0064A1F4,         "CreateEventA",               "KERNEL32" },
    { 0x0064A1F8,         "CloseHandle",                "KERNEL32" },
    { 0x0064A1FC,         "GetExitCodeThread",          "KERNEL32" },
    { 0x0064A200,         "GetVersionExA",              "KERNEL32" },
    { 0x0064A204,         "ResumeThread",               "KERNEL32" },
    { 0x0064A208,         "SuspendThread",              "KERNEL32" },
    { 0x0064A20C,         "GetDriveTypeA",              "KERNEL32" },
    { 0x0064A210,         "SetErrorMode",               "KERNEL32" },
    { 0x0064A214,         "GetLogicalDrives",           "KERNEL32" },
    { 0x0064A218,         "LoadLibraryA",               "KERNEL32" },
    { 0x0064A21C,         "GetVolumeInformationA",      "KERNEL32" },
    { 0x0064A220,         "GlobalMemoryStatus",         "KERNEL32" },
    { 0x0064A224,         "FreeLibrary",                "KERNEL32" },
    { 0x0064A228,         "GetProcAddress",             "KERNEL32" },
    { 0x0064A22C,         "GetCurrentProcess",          "KERNEL32" },
    { 0x0064A230,         "FindClose",                  "KERNEL32" },
    { 0x0064A234,         "OutputDebugStringA",         "KERNEL32" },
    { 0x0064A238,         "MulDiv",                     "KERNEL32" },
    { 0x0064A23C,         "QueryPerformanceFrequency",  "KERNEL32" },
    { 0x0064A240,         "FindFirstFileA",             "KERNEL32" },
    { 0x0064A244,         "FindNextFileA",              "KERNEL32" },
    { 0x0064A248,         "TlsAlloc",                   "KERNEL32" },
    { 0x0064A24C,         "QueryPerformanceCounter",    "KERNEL32" },
    { 0x0064A250,         "TlsFree",                    "KERNEL32" },
    { 0x0064A254,         "GetEnvironmentStrings",      "KERNEL32" },
    { 0x0064A258,         "GetEnvironmentStringsW",     "KERNEL32" },
    { 0x0064A25C,         "HeapReAlloc",                "KERNEL32" },
    { 0x0064A260,         "MapViewOfFile",              "KERNEL32" },
    { 0x0064A264,         "UnmapViewOfFile",            "KERNEL32" },
    { 0x0064A268,         "GetCurrentDirectoryA",       "KERNEL32" },
    { 0x0064A26C,         "CreateFileMappingA",         "KERNEL32" },
    { 0x0064A270,         "SetCurrentDirectoryA",       "KERNEL32" },
    { 0x0064A274,         "GetFileSize",                "KERNEL32" },
    { 0x0064A278,         "RaiseException",             "KERNEL32" },
    { 0x0064A280,         "FindWindowA",                "USER32" },
    { 0x0064A284,         "LoadIconA",                  "USER32" },
    { 0x0064A288,         "RegisterClassA",             "USER32" },
    { 0x0064A28C,         "GetSystemMetrics",           "USER32" },
    { 0x0064A290,         "CreateWindowExA",            "USER32" },
    { 0x0064A294,         "ShowWindow",                 "USER32" },
    { 0x0064A298,         "UpdateWindow",               "USER32" },
    { 0x0064A29C,         "ShowCursor",                 "USER32" },
    { 0x0064A2A0,         "PeekMessageA",               "USER32" },
    { 0x0064A2A4,         "PostMessageA",               "USER32" },
    { 0x0064A2A8,         "MoveWindow",                 "USER32" },
    { 0x0064A2AC,         "DefWindowProcA",             "USER32" },
    { 0x0064A2B0,         "MapVirtualKeyA",             "USER32" },
    { 0x0064A2B4,         "GetKeyNameTextA",            "USER32" },
    { 0x0064A2B8,         "GetAsyncKeyState",           "USER32" },
    { 0x0064A2BC,         "LoadImageA",                 "USER32" },
    { 0x0064A2C0,         "SetWindowTextA",             "USER32" },
    { 0x0064A2C4,         "GetClientRect",              "USER32" },
    { 0x0064A2C8,         "ClientToScreen",             "USER32" },
    { 0x0064A2CC,         "MessageBoxA",                "USER32" },
    { 0x0064A2D0,         "DestroyWindow",              "USER32" },
    { 0x0064A2D4,         "PostThreadMessageA",         "USER32" },
    { 0x0064A2D8,         "LoadStringA",                "USER32" },
    { 0x0064A2DC,         "GetMessageA",                "USER32" },
    { 0x0064A2E0,         "TranslateMessage",           "USER32" },
    { 0x0064A2E4,         "DispatchMessageA",           "USER32" },
    { 0x0064A2E8,         "PostQuitMessage",            "USER32" },
    { 0x0064A2F0,         "timeGetTime",                "WINMM"  }
};

void LoadImports()
{
    for (const ImportPointer& import : gImports)
    {
        HMODULE hMod = LoadLibrary(import.mDllName);
        FARPROC proc = GetProcAddress(hMod, import.mFuncName);
        if (!proc)
        {
            MGS_FATAL("Import failed");
        }
        DWORD* target = (DWORD*)import.mTarget;
        *target = (DWORD)proc;
    }

}

void InitRealGameSections()
{
    // This has to be at 0x00041000 otherwise the addresses in the real games code will be wrong
    // e.g it will expect a var to be at 0x00041000+N which will be false if its base is not 0x401000
    if (reinterpret_cast<DWORD>(gRealGameSectionsBuffer) != 0x401000)
    {
        std::string err = "Expected base address of gRealGameSectionsBuffer to be 0x00041000 but was: " + std::to_string(reinterpret_cast<DWORD>(gRealGameSectionsBuffer));
        MGS_FATAL(err.c_str());
    }

    // The section is read only, we need to make it read/write + execute
    DWORD oldProt = 0;
    if (!VirtualProtect(gRealGameSectionsBuffer, _countof(gRealGameSectionsBuffer), PAGE_EXECUTE_READWRITE , &oldProt))
    {
        std::string err = "Virtual protect failed with [" + std::to_string(::GetLastError()) + "]";
        MGS_FATAL(err.c_str());
    }


    /*
    Section info obtained from PE-Bear.
    [Name]      [Raw Addr]     [Raw size]    [Virtual size]  [Characteristics]
    .text       0x1000         0x249000      0x248122        0x60000020
    .rdata      0x249000       0x6000        0x51e3          0x40000040
    .data       0x250000       0x6c000       0x36239c        0xC0000040
    .rsrc       0x2bc000       0x4000        0x31be          0x40000040

    .text  00401000 0064A000 R . X . L para 0001 public CODE 32 0000 0000 0003 FFFFFFFF FFFFFFFF
    .idata 0064A000 0064A2F8 R . . . L para 0002 public DATA 32 0000 0000 0003 FFFFFFFF FFFFFFFF
    .rdata 0064A2F8 00650000 R . . . L para 0002 public DATA 32 0000 0000 0003 FFFFFFFF FFFFFFFF
    .data  00650000 009B3000 R W . . L para 0003 public DATA 32 0000 0000 0003 FFFFFFFF FFFFFFFF
    */

    // Load up the real/original MGSI.exe
    const char* exepath = "C:\\Program Files (x86)\\Metal Gear Solid\\MGSI.exe";
    FILE* fd = fopen(exepath, "rb");
    if (!fd)
    {
        MGS_FATAL("Failed to open exe");
    }

    fseek(fd, 0x1000, SEEK_SET);
    
    std::vector<BYTE> buf(0x249000);

    char* ptr = (char*)0x401000;
    
    fread(buf.data(), 1, 0x249000, fd);

  
    memcpy(ptr, buf.data(), buf.size());

    fseek(fd, 0x249000, SEEK_SET);
    fread((void*)0x0064A000, 1, 0x6000, fd);

    fseek(fd, 0x250000, SEEK_SET);
    fread((void*)0x0064A2F8, 1, 0x6c000, fd);

   // 0x009B239C - 0x401000 = 0x5B139C

    // 24e154 size b4
    fseek(fd, 0x24e154, SEEK_SET);
    buf.resize(0xb4);
    fread(buf.data(), 1, buf.size(), fd);




    /*
    PIMAGE_IMPORT_DESCRIPTOR pImports = (PIMAGE_IMPORT_DESCRIPTOR)buf.data();

    char* pName = (char*)buf.data() + (pImports->Name - 0x24e154);


    DWORD* p = (DWORD*)0x9ADDA0;
    *p = 7;
   */

   

    fclose(fd);

}
