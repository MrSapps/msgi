#include "stdafx.h"
#include <gmock/gmock.h>

bool IsMgsi()
{
    return false;
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
