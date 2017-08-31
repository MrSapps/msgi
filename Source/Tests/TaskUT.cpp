#include "stdafx.h"
#include <gmock/gmock.h>
#include "Task.hpp"
#include "Sound.hpp"
#include <atomic>

#pragma comment(lib, "dsound.lib")

std::atomic<int> gThreadProcCalls;

unsigned int __stdcall TaskThreadProc(void(*)(void))
{
    gThreadProcCalls++;
    return 7;
}

// TODO: Need to rip the thread proc array
TEST(Task, Init)
{
    gThreadProcCalls = 0;
    Task_Init(30, TaskThreadProc);
    while (gThreadProcCalls != 1) {}
}

// TODO: Fails on AppVeyor - probably need to mock the IDirectSound obj
TEST(Sound, DISABLED_Init_ShutDown)
{
    ASSERT_NE(0, Sound_Init(::GetDesktopWindow()));
    Sound_ShutDown();
}


// TODO: Needs array ripping
TEST(Sound, DISABLED_Sound_SetMusicVolume)
{
    for (int i = 0; i <= 100; i++)
    {
        Sound_SetMusicVolume(i); // TODO: Check ret
    }
}

TEST(Sound, Sound_HexCharToInt)
{
    // Chars 0 to 9 converted to number
    ASSERT_EQ(0x0, Sound_HexCharToInt('0'));
    ASSERT_EQ(0x1, Sound_HexCharToInt('1'));

    // Strings A-F an a-f converted to number
    ASSERT_EQ(0xA, Sound_HexCharToInt('a'));
    ASSERT_EQ(0xA, Sound_HexCharToInt('A'));

    ASSERT_EQ(0xB, Sound_HexCharToInt('b'));
    ASSERT_EQ(0xB, Sound_HexCharToInt('B'));

    ASSERT_EQ(0xC, Sound_HexCharToInt('c'));
    ASSERT_EQ(0xC, Sound_HexCharToInt('C'));

    ASSERT_EQ(0xF, Sound_HexCharToInt('f'));
    ASSERT_EQ(0xF, Sound_HexCharToInt('F'));

    // Anything else is 0
    ASSERT_EQ(0x0, Sound_HexCharToInt('z'));
}
