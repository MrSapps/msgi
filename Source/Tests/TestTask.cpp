#include "stdafx.h"
#include <gmock/gmock.h>
#include "Task.hpp"

unsigned int __stdcall TaskThreadProc(void *)
{
    return 7;
}

TEST(Task, Init)
{
    //Task_Init(30, TaskThreadProc);
}

