
#include <Windows.h>
#include <iostream>
#include "functions.h"


char AmmoOpCode[] = "\x90\x90";
char normalOpCode[] = "\xFF\x0E";

bool unlimitedAmmo = false;

DWORD ammoAdress;

DWORD GetAddress()
{
    DWORD ammoAddress = FindPattern((LPTSTR)"ac_client.exe", (LPTSTR)"\x89\x0A\x8B\x76\x14\xFF\x0E\x57\x8B\x7C\x24\x14\x8D\x74\x24\x28\xE8\x00\x00\x00\x00", (LPTSTR)"xxxxxxxxxxxxxxxxx????");

    ammoAddress += 5;

    return ammoAddress;
}

DWORD WINAPI UnlimitedAmmo(LPVOID lp)
{
    ammoAdress = GetAddress();
    while (true)
    {
        if (GetAsyncKeyState(VK_F8) && 1)
        {
            if (!unlimitedAmmo)
            {
                WriteMem(ammoAdress, AmmoOpCode, 2);
                unlimitedAmmo = true;
            }
            else {
                WriteMem(ammoAdress, normalOpCode, 2);
                unlimitedAmmo = false;
            }
        }
        Sleep(100);
    }
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, UnlimitedAmmo, hModule, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

