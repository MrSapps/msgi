#include "stdafx.h"
#include "Input.hpp"
#include "MgsFunction.hpp"
#include <assert.h>

#define INPUT_IMPL true

#define USE_DINPUT8 1

#if USE_DINPUT8
#define DIRECTINPUT_VERSION 0x800
#else
#define DIRECTINPUT_VERSION 0x700
#endif

#include <dinput.h>

MGS_VAR(1, 0x71D664, LPDIRECTINPUT8, pDirectInput, nullptr);
MGS_VAR(1, 0x71D66C, LPDIRECTINPUTDEVICE8, pJoystickDevice, nullptr);
MGS_VAR(1, 0x71D668, LPDIRECTINPUTDEVICE8, pMouseDevice, nullptr);
MGS_VAR(1, 0x71D420, DIDEVICEINSTANCEA, JoystickDeviceInfos, {});
MGS_VAR(1, 0x71D1D8, DIDEVCAPS, JoystickDeviceCaps, {});
MGS_ARY(1, 0x6571F4, DWORD, 14, dword_6571F4, {});// TODO: Check 14 is big enough


char* sidewinderEtc = (char*)0x657298; // TODO: Dump array
DWORD* dword_65726C = (DWORD*)0x65726C;
char* buttonNames = (char*)0x65510C; // TODO: Dump array
char* buttonList = (char*)0x654A98; // TODO: Dump array
MGS_VAR(1, 0x71D68C, int, nJoystickDeviceObjects, 0);
MGS_VAR(1, 0x6FD1DC, DWORD, dword_6FD1DC, 0);
MGS_VAR(1, 0x71D670, DWORD, dword_71D670, 0);
MGS_VAR(1, 0x71D790, DWORD, dword_71D790, 0);
MGS_VAR(1, 0x71D798, DWORD, gJoyStickId_dword_71D798, 0);
MGS_VAR(1, 0x71D41C, int, dword_71D41C, 0);
MGS_ARY(1, 0x65714C, DWORD, 14, dword_65714C, {});
MGS_ARY(1, 0x657184, DWORD, 14, dword_657184, {});
MGS_VAR(1, 0x71D79C, DWORD, dword_71D79C, 0);
MGS_ARY(1, 0x6571BC, DWORD, 14, dword_6571BC, {});// TODO: Check 14 is big enough
MGS_ARY(1, 0x71D690, char, 256, char_71D69, {});


// WinMain.cpp
extern DWORD& dword_717348; 
extern HINSTANCE& gHInstance;
extern DWORD& gWindowedMode;
extern HWND& gHwnd;
extern DWORD& gActive_dword_688CDC;
extern DWORD& dword_73490C;
extern DWORD& dword_734908;

MSG_FUNC_NOT_IMPL(0x00553090, signed int __stdcall(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter), DirectInputCreateExMGS);

MGS_ARY(1, 0x71D30C, DWORD, 65, gButtonStates_dword_71D30C, {});
MGS_ARY(1, 0x71D204, DWORD, 55, gKeyBoardButtonStates_dword_71D204, {});
MGS_VAR(1, 0x721E44, DWORD, dword_721E44, 0);
MGS_VAR(1, 0x734910, DWORD, dword_734910, 0);
MGS_VAR(1, 0x734914, DWORD, dword_734914, 0);
MGS_VAR(1, 0x71D680, DWORD, dword_71D680, 0);
MGS_VAR(1, 0x71D688, DWORD, dword_71D688, 0);
MGS_VAR(1, 0x71D684, DWORD, dword_71D684, 0);
MGS_VAR(1, 0x71D418, DWORD, dword_71D418, 0);
MGS_VAR(1, 0x71D1EC, DWORD, dword_71D1EC, 0);
MGS_VAR(1, 0x657148, DWORD, dword_657148, 0);

// 0x43BD6E
HRESULT __cdecl Input_Read_43BD6E()
{
    DIMOUSESTATE mouseState;
    HRESULT hr;
    DIJOYSTATE joystickState;

    dword_721E44 = 0;
    if (pMouseDevice)
    {
        hr = DIERR_INPUTLOST;
        while (hr == DIERR_INPUTLOST)
        {
            hr = pMouseDevice->GetDeviceState(16, &mouseState);
            if (hr == DIERR_INPUTLOST)
            {
                hr = pMouseDevice->Acquire();
                if (hr < 0)
                    return hr;
            }
        }
        if (hr < 0)
            return hr;
        dword_734908 = mouseState.lX;
        dword_73490C = mouseState.lY;
        *(&dword_71D670 + 3) = mouseState.lZ; // TODO: Type isn't right
        dword_734910 = (unsigned __int8)(mouseState.rgbButtons[0] & 0x80) != 0;
        dword_734914 = (unsigned __int8)(mouseState.rgbButtons[1] & 0x80) != 0;
        if (!(mouseState.rgbButtons[0] & 0x80) && !dword_734914)
            dword_71D418 = 1;
        if (dword_734910 || dword_734914 && dword_71D418)
            dword_721E44 = 1;
        gButtonStates_dword_71D30C[56] = mouseState.rgbButtons[0] & 0x80;
        gButtonStates_dword_71D30C[57] = mouseState.rgbButtons[1] & 0x80;
        gButtonStates_dword_71D30C[58] = mouseState.rgbButtons[2] & 0x80;
        gButtonStates_dword_71D30C[59] = mouseState.rgbButtons[3] & 0x80;
        for (int i = 60; i <= 65; ++i)
        {
            switch (i)
            {
            case 60:
                if (mouseState.lX >= -15)
                    gButtonStates_dword_71D30C[i] = 0;
                else
                    gButtonStates_dword_71D30C[i] = 128;
                break;
            case 61:
                if (mouseState.lX <= 15)
                    gButtonStates_dword_71D30C[i] = 0;
                else
                    gButtonStates_dword_71D30C[i] = 128;
                break;
            case 62:
                if (mouseState.lY >= -15)
                    gButtonStates_dword_71D30C[i] = 0;
                else
                    gButtonStates_dword_71D30C[i] = 128;
                break;
            case 63:
                if (mouseState.lY <= 15)
                    gButtonStates_dword_71D30C[i] = 0;
                else
                    gButtonStates_dword_71D30C[i] = 128;
                break;
            case 64:
                if (mouseState.lZ <= 15)
                    gButtonStates_dword_71D30C[i] = 0;
                else
                    gButtonStates_dword_71D30C[i] = 128;
                break;
            case 65:
                if (mouseState.lZ >= -15)
                    gButtonStates_dword_71D30C[i] = 0;
                else
                    gButtonStates_dword_71D30C[i] = 128;
                break;
            }
        }
    }
    if (pJoystickDevice)
    {
        do
        {
            hr = pJoystickDevice->Poll();
            if (hr < 0)
                printf("$jim poll crashed\n");
            hr = pJoystickDevice->GetDeviceState(80, &joystickState);
            if (hr == DIERR_INPUTLOST)
            {
                hr = pJoystickDevice->Acquire();
                if (hr < 0)
                    return hr;
            }
        } while (hr == DIERR_INPUTLOST);
        if (hr < 0)
            return hr;
        dword_71D680 = 0;
        dword_71D684 = 0;
        for (DWORD i = 0; i < dword_657148; ++i)
        {
            if (i < 0 || i >= 32)
            {
                switch (i)
                {
                case 44:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.rglSlider[0];
                    break;
                case 45:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.rglSlider[0];
                    break;
                case 46:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.rglSlider[1];
                    break;
                case 47:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.rglSlider[1];
                    break;
                case 38:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lRx;
                    break;
                case 39:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lRx;
                    break;
                case 40:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lRy;
                    break;
                case 41:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lRy;
                    break;
                case 42:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lRz;
                    break;
                case 43:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lRz;
                    break;
                case 32:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lX;
                    break;
                case 33:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lX;
                    break;
                case 34:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lY;
                    break;
                case 35:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lY;
                    break;
                case 36:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lZ;
                    break;
                case 37:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.lZ;
                    break;
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                    gKeyBoardButtonStates_dword_71D204[i] = joystickState.rgdwPOV[0];
                    break;
                default:
                    continue;
                }
            }
            else if (joystickState.rgbButtons[i] & 0x80)
            {
                gKeyBoardButtonStates_dword_71D204[i] = 128;
                if (dword_71D688)
                    dword_721E44 = 1;
                dword_71D684 = 1;
            }
            else
            {
                gKeyBoardButtonStates_dword_71D204[i] = 0;
            }
        }
        if (!dword_71D680 && !dword_71D684)
        {
            dword_71D680 = 1;
            dword_71D688 = 1;
        }
        for (DWORD i = 0; i < dword_657148; ++i)
        {
            if (i < 0 || i >= 32)
            {
                switch (i)
                {
                case 32:
                    if (joystickState.lX <= 250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 33:
                    if (joystickState.lX >= -250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 34:
                    if (joystickState.lY <= 250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 35:
                    if (joystickState.lY >= -250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 36:
                    if (joystickState.lZ <= 250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 37:
                    if (joystickState.lZ >= -250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 38:
                    if (joystickState.lRx >= -250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 39:
                    if (joystickState.lRx <= 250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 40:
                    if (joystickState.lRy >= -250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 41:
                    if (joystickState.lRy <= 250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 42:
                    if (joystickState.lRz >= -250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 43:
                    if (joystickState.lRz <= 250)
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 48:
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_152;
                    if (joystickState.rgdwPOV[0])
                        gButtonStates_dword_71D30C[i] = 0;
                    else
                        gButtonStates_dword_71D30C[i] = 128;
                    break;
                case 49:
                LABEL_152 :
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_157;
                          if (joystickState.rgdwPOV[0] == 4500)
                              gButtonStates_dword_71D30C[i] = 128;
                          else
                              gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 50:
                LABEL_157 :
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_162;
                          if (joystickState.rgdwPOV[0] == 9000)
                              gButtonStates_dword_71D30C[i] = 128;
                          else
                              gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 51:
                LABEL_162 :
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_167;
                          if (joystickState.rgdwPOV[0] == 13500)
                              gButtonStates_dword_71D30C[i] = 128;
                          else
                              gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 52:
                LABEL_167 :
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_172;
                          if (joystickState.rgdwPOV[0] == 18000)
                              gButtonStates_dword_71D30C[i] = 128;
                          else
                              gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 53:
                LABEL_172 :
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_177;
                          if (joystickState.rgdwPOV[0] == 22500)
                              gButtonStates_dword_71D30C[i] = 128;
                          else
                              gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 54:
                LABEL_177 :
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_182;
                          if (joystickState.rgdwPOV[0] == 27000)
                              gButtonStates_dword_71D30C[i] = 128;
                          else
                              gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 55:
                LABEL_182 :
                    if ((unsigned int)dword_71D1EC < 1)
                        goto LABEL_187;
                          if (joystickState.rgdwPOV[0] == 31500)
                              gButtonStates_dword_71D30C[i] = 128;
                          else
                              gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 44:
                LABEL_187 :
                    gButtonStates_dword_71D30C[i] = 0;
                          break;
                case 45:
                    gButtonStates_dword_71D30C[i] = 0;
                    break;
                case 46:
                    gButtonStates_dword_71D30C[i] = 0;
                    break;
                case 47:
                    gButtonStates_dword_71D30C[i] = 0;
                    break;
                default:
                    continue;
                }
            }
            else
            {
                gButtonStates_dword_71D30C[i] = joystickState.rgbButtons[i];
            }
        }
    }
    return 0;
}
MSG_FUNC_IMPLEX(0x43BD6E, Input_Read_43BD6E, INPUT_IMPL);

// 0x0043B0B3
BOOL WINAPI Input_Enum_Buttons_sub_43B0B3(LPCDIDEVICEOBJECTINSTANCE /*lpddoi*/, LPVOID /*pvRef*/)
{
    ++nJoystickDeviceObjects;
    return DIENUM_CONTINUE;
}
MSG_FUNC_IMPLEX(0x0043B0B3, Input_Enum_Buttons_sub_43B0B3, INPUT_IMPL);

// 0x0043B0C8
BOOL WINAPI Input_Enum_Axis_43B0C8(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID /*pvRef*/)
{
    static_assert(sizeof(DIPROPHEADER) == 16, "Wrong DIPROPHEADER size");
    static_assert(sizeof(DIPROPRANGE) == 24, "Wrong DIPROPRANGE size");

    DIPROPRANGE p = {};
    p.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    p.diph.dwSize = sizeof(DIPROPRANGE);
    p.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    p.diph.dwHow = DIPH_BYOFFSET;
    p.diph.dwObj = lpddoi->dwOfs;
    p.lMin = -1000;
    p.lMax  = 1000;

    if (SUCCEEDED(pJoystickDevice->SetProperty(DIPROP_RANGE, &p.diph)))
    {
        switch (lpddoi->dwOfs)
        {
        case 0u:
            ++dword_71D79C;
            break;

        case 4u:
            ++dword_71D79C;
            break;

        case 8u:
            ++dword_71D79C;
            break;

        case 12u:
            ++dword_71D79C;
            break;

        case 16u:
            ++dword_71D79C;
            break;

        case 20u:
            ++dword_71D79C;
            break;

        case 24u:
            ++dword_71D79C;
            break;

        case 28u:
            ++dword_71D79C;
            break;
        }

        return DIENUM_CONTINUE;
    }
    
    return DIENUM_STOP;
}
MSG_FUNC_IMPLEX(0x0043B0C8, Input_Enum_Axis_43B0C8, INPUT_IMPL);

// 0x0043B078
BOOL __stdcall Input_EnumDevicesCallback(LPCDIDEVICEINSTANCEA lpddi, PVOID /*pvRef*/)
{
    // Stop when worked
    HRESULT hr = pDirectInput->CreateDevice(lpddi->guidInstance, &pJoystickDevice, NULL);
    return !SUCCEEDED(hr);
}
MSG_FUNC_IMPLEX(0x0043B078, Input_EnumDevicesCallback, INPUT_IMPL);

// 0x43C716
int __cdecl Input_Shutdown_sub_43C716()
{
    if (pJoystickDevice)
    {
        pJoystickDevice->Unacquire();
        pJoystickDevice->Release();
        pJoystickDevice = 0;
    }

    if (pMouseDevice)
    {
        pMouseDevice->Unacquire();
        pMouseDevice->Release();
        pMouseDevice = 0;
    }

    if (pDirectInput)
    {
        pDirectInput->Release();
        pDirectInput = 0;
    }
    return 0;
}
MSG_FUNC_IMPLEX(0x43C716, Input_Shutdown_sub_43C716, INPUT_IMPL);


// 0x0043B1D1
//MSG_FUNC_NOT_IMPL(0x0043B1D1, int __cdecl(HWND), InitDirectInput);
int __cdecl Input_Init(HWND hWnd)
{
    char productName[0x80];
    char instanceName[0x80];
    dword_71D670 = 0;
    //fputs("InitDirectInput {\n", gLogFile);
    // I'll do log prints later
#if USE_DINPUT8
    HRESULT hr = DirectInput8Create(gHInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pDirectInput, 0);
#else
    HRESULT hr = DirectInputCreateExMGS(gHInstance, DIRECTINPUT_VERSION, IID_IDirectInput7A_MGS, (LPVOID*)&pDirectInput, 0);
#endif

    if (hr < 0)
        return hr;

#if USE_DINPUT8
    hr = pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, Input_EnumDevicesCallback, 0, DIEDFL_ATTACHEDONLY);
#else
    hr = pDirectInput->EnumDevices(DIDEVTYPE_JOYSTICK, Input_EnumDevicesCallback, 0, DIEDFL_ATTACHEDONLY);
#endif

    if (hr >= 0)
    {
        if (pJoystickDevice != 0)
        {
            memset(&JoystickDeviceInfos, 0, sizeof(DIDEVICEINSTANCEA));
            assert(sizeof(DIDEVICEINSTANCEA) == 0x244);
            JoystickDeviceInfos.dwSize = sizeof(DIDEVICEINSTANCEA);
            HRESULT hGetInfosRes = pJoystickDevice->GetDeviceInfo(&JoystickDeviceInfos);
            hr = pJoystickDevice->SetDataFormat(&c_dfDIJoystick);
            if (hr >= 0)
            {
                hr = pJoystickDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
                if (hr >= 0)
                {
                    memset(&JoystickDeviceCaps, 0, 0x2Cu);
                    JoystickDeviceCaps.dwSize = 0x2C;
                    hr = pJoystickDevice->GetCapabilities(&JoystickDeviceCaps);
                    if (hr >= 0)
                    {
                        pJoystickDevice->EnumObjects(Input_Enum_Axis_43B0C8, hWnd, DIDFT_AXIS);
                        pJoystickDevice->EnumObjects(Input_Enum_Buttons_sub_43B0B3, hWnd, DIDFT_BUTTON);
#if USE_DINPUT8
                        do
#endif
                        {
                            hr = pJoystickDevice->Acquire();
#if USE_DINPUT8
                        } while (hr == E_ACCESSDENIED);
#endif

                        if (hr >= 0)
                        {
                            if (hGetInfosRes >= 0)
                            {
                                // TODO: Make 0x71D690 a var
                                strcpy(char_71D69, JoystickDeviceInfos.tszInstanceName);

                                for (int i = 0; i < 6; i++)
                                {
                                    int var14 = 1;
                                    strcpy(productName, JoystickDeviceInfos.tszProductName);
                                    _strlwr(productName);
                                    strcpy(instanceName, JoystickDeviceInfos.tszInstanceName);
                                    _strlwr(instanceName);

                                    for (int j = 0; j < 5; j++)
                                    {
                                        // TODO: Figure out the size of sidewinderEtc
                                        size_t offset = i * 0x140 + j * 0x40;
                                        if (strstr(productName, &sidewinderEtc[offset]) == 0 && strstr(instanceName, &sidewinderEtc[offset]) == 0)
                                        {
                                            var14 = 0;
                                        }
                                    }

                                    if (var14 != 0)
                                    {
                                        if (i == 5)
                                            i = 4;

                                        dword_71D790 = 1;
                                        dword_71D41C = dword_65726C[i * 2];
                                        gJoyStickId_dword_71D798 = i + 1;

                                        for (int nButton = 0; nButton < 0x38; nButton++)
                                        {
                                            size_t offset = i * 0x672 + nButton * 0x19;
                                            strcpy(&buttonList[nButton * 0x19], &buttonNames[offset]);
                                        }
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                dword_71D790 = 0;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = 0;
                                    dword_657184[i] = 0;
                                }
                            }
                            if (gJoyStickId_dword_71D798 == 5)
                            {
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_6571BC[i] = 0xFF;
                                    dword_6571F4[i] = 0xFF;
                                }
                                dword_6571BC[0] = dword_6571F4[0] = 0;
                                dword_6571BC[1] = dword_6571F4[1] = 1;
                                dword_6571BC[2] = dword_6571F4[2] = 2;
                                dword_6571BC[3] = dword_6571F4[3] = 3;
                                dword_6571BC[4] = dword_6571F4[4] = 6;
                                dword_6571BC[5] = dword_6571F4[5] = 6;
                                dword_6571BC[6] = dword_6571F4[6] = 7;
                                dword_6571BC[7] = dword_6571F4[7] = 7;
                                dword_6571BC[8] = dword_6571F4[8] = 4;
                                dword_6571BC[9] = dword_6571F4[9] = 0x21;
                                dword_6571BC[10] = dword_6571F4[10] = 0x20;
                                dword_6571BC[11] = dword_6571F4[11] = 0x23;
                                dword_6571BC[12] = dword_6571F4[12] = 0x22;
                                dword_6571BC[13] = dword_6571F4[13] = 5;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = dword_657184[i];
                                }
                            }
                            else if (gJoyStickId_dword_71D798 != 1 && gJoyStickId_dword_71D798 != 4)
                            {
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_6571BC[i] = 0xFF;
                                    dword_6571F4[i] = 0xFF;
                                }
                                int var124 = 0;
                                for (int i = 0; i < 14; i++)
                                {
                                    if (dword_71D790 != 0 && var124 == dword_71D41C)
                                    {
                                        var124++;
                                    }
                                    if (var124 == nJoystickDeviceObjects)
                                        break;

                                    if (i == 9)
                                        i = 13;

                                    dword_6571BC[i] = var124;
                                    dword_6571F4[i] = var124;

                                    var124++;
                                }
                                dword_6571BC[9] = dword_6571F4[9] = 0x21;
                                dword_6571BC[10] = dword_6571F4[10] = 0x20;
                                dword_6571BC[11] = dword_6571F4[11] = 0x23;
                                dword_6571BC[12] = dword_6571F4[12] = 0x22;
                                for (int i = 0; i < 14; i++)
                                {
                                    dword_65714C[i] = 0;
                                    dword_657184[i] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (hr < 0 || pJoystickDevice == 0)
    {
        for (unsigned int i = 0; i < dword_6FD1DC; i++)
        {
            dword_6571F4[i] = 0xFF;
        }
    }

    // 0x43BBEC
    hr = pDirectInput->CreateDevice(GUID_SysMouse, &pMouseDevice, 0);
    if (hr < 0)
        return hr;

    hr = pMouseDevice->SetDataFormat(&c_dfDIMouse);
    if (hr < 0)
        return hr;

    if (gWindowedMode != 0)
    {
        hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    }
    else
    {
        hr = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    }
    if (hr < 0)
        return hr;

    hr = pMouseDevice->Acquire();

    return 0;
}
MSG_FUNC_IMPLEX(0x0043B1D1, Input_Init, INPUT_IMPL);

// 0x0042D69E
int __cdecl Input_Start()
{
    int result; // eax@1

    dword_717348 = 0;
    result = Input_Init(gHwnd);
    if (result < 0)
        result = printf("$jim failed to init direct input");
    return result;
}
MSG_FUNC_IMPLEX(0x0042D69E, Input_Start, INPUT_IMPL);

void __cdecl Input_AcquireOrUnAcquire()
{
    if (pMouseDevice)
    {
        if (gActive_dword_688CDC)
        {
            pMouseDevice->Acquire();
        }
        else
        {
            pMouseDevice->Unacquire();
        }
    }
    if (pJoystickDevice)
    {
        if (gActive_dword_688CDC)
        {
            if (pJoystickDevice->Acquire())
            {
                printf("$jim - cannot acquire joystick\n");
            }
        }
        else
        {
            pJoystickDevice->Unacquire();
        }
    }
}
MSG_FUNC_IMPLEX(0x0043BCF0, Input_AcquireOrUnAcquire, INPUT_IMPL);
