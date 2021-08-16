#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <Psapi.h>


void ShowMessage(DWORD someText)
{
    char buffer[2048];
    sprintf(buffer, "Address: %02x", someText);
    MessageBox(NULL, buffer, "He Thong", MB_OK);
}

MODULEINFO GetModuleInfo(char* szModule)
{
    MODULEINFO modInfo = { 0 };
    HMODULE hModule = GetModuleHandle(szModule);
    if (hModule == INVALID_HANDLE_VALUE)
        return modInfo;
    else
        GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    return modInfo;
}

void WriteMem(uintptr_t addressToWrite, char* valueToWrite, int byteNum)
{
    unsigned long oldProtection;;

    VirtualProtect((LPVOID)addressToWrite, byteNum, PAGE_EXECUTE_READWRITE, &oldProtection);

    memcpy((LPVOID)addressToWrite, valueToWrite, byteNum);

    VirtualProtect((LPVOID)addressToWrite, byteNum, oldProtection, NULL);
}

DWORD FindPattern(char* module, char* pattern, char* mask)
{
    MODULEINFO moduleInfo = GetModuleInfo(module);

    DWORD base = (DWORD)moduleInfo.lpBaseOfDll;
    DWORD size = (DWORD)moduleInfo.SizeOfImage;

    DWORD patternLength = (DWORD)strlen(mask);

    for (DWORD i = 0; i < size - patternLength; i++)
    {
        bool found = true;
        for (DWORD j = 0; j < patternLength; j++)
        {
            found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
        }

        if (found)
            return base + i;
    }
    return 0xDEADBEAF;
}