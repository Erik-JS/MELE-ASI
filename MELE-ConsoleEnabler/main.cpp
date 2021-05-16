#include <windows.h>
#include <stdio.h>

DWORD WINAPI Start(LPVOID lpParam)
{
    Sleep(1000);
    DWORD64 target1 = 0, target2 = 0, target3 = 0;
    char filename[FILENAME_MAX];
    HMODULE hMain = GetModuleHandle(NULL);
    GetModuleFileName(hMain, filename, sizeof(filename));
    int gamenumberindex = strlen(filename) - 5;
    if(filename[gamenumberindex] == '1') target1 = 0x14168E358;
    else if(filename[gamenumberindex] == '2') target1 = 0x141679458;
    else if(filename[gamenumberindex] == '3') target1 = 0x1417BD088;
    target2 = target1 + 0x20;
    target3 = target2 + 0x200;
    DWORD dwProtect = NULL;
    VirtualProtect((void*)(target1), 0x4, PAGE_READWRITE, &dwProtect );
    VirtualProtect((void*)(target2), 0x4, PAGE_READWRITE, &dwProtect );
    *(DWORD64*)(target1) = *(DWORD64*)(target3);
    *(DWORD64*)(target2) = *(DWORD64*)(target3);
    VirtualProtect((void*)(target1), 0x4, dwProtect, &dwProtect );
    VirtualProtect((void*)(target2), 0x4, dwProtect, &dwProtect );
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, 0, 0, 0);
    }
    return TRUE;
}
