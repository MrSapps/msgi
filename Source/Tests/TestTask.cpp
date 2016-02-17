#include "stdafx.h"
#include <gmock/gmock.h>
#include "Task.hpp"
#include "Sound.hpp"

#pragma comment(lib, "dsound.lib")

unsigned int __stdcall TaskThreadProc(void *)
{
    return 7;
}

TEST(Task, Init)
{
    //Task_Init(30, TaskThreadProc);
}

TEST(Sound, Init_ShutDown)
{
    ASSERT_NE(0, Sound_Init(::GetDesktopWindow()));
    Sound_ShutDown();
}
