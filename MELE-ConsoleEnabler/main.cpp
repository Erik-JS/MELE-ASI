#include <windows.h>
#include <stdio.h>

unsigned char pattern1[64] = {
	0x69, 0x00, 0x6E, 0x00, 0x74, 0x00, 0x41, 0x00, 0x57, 0x00, 0x6F, 0x00,
	0x72, 0x00, 0x6C, 0x00, 0x64, 0x00, 0x49, 0x00, 0x6E, 0x00, 0x66, 0x00,
	0x6F, 0x00, 0x65, 0x00, 0x78, 0x00, 0x65, 0x00, 0x63, 0x00, 0x49, 0x00,
	0x73, 0x00, 0x53, 0x00, 0x68, 0x00, 0x69, 0x00, 0x70, 0x00, 0x70, 0x00,
	0x69, 0x00, 0x6E, 0x00, 0x67, 0x00, 0x42, 0x00, 0x75, 0x00, 0x69, 0x00,
	0x6C, 0x00, 0x64, 0x00
};

bool DataCompare(const BYTE* OpCodes, const BYTE* Mask, const char* StrMask)
{
	while (*StrMask)
	{
		if(*StrMask == 'x' && *OpCodes != *Mask )
			return false;
		++StrMask;
		++OpCodes;
		++Mask;
	}
	return true;
}

DWORD64 FindPattern(DWORD64 StartAddress, DWORD64 CodeLen, BYTE* Mask, const char* StrMask, unsigned short ignore)
{
	unsigned short Ign = 0;
	DWORD64 i = 0;
	while (Ign <= ignore)
	{
		if(DataCompare((BYTE*)(StartAddress + i++), Mask,StrMask))
			++Ign;
		else if (i>=CodeLen)
			return 0;
	}
	return StartAddress + i - 1;
}

DWORD WINAPI Start(LPVOID lpParam)
{
    DWORD64 exeBaseAddress = (DWORD64)GetModuleHandle(NULL);
    DWORD64 bResult1 = FindPattern(exeBaseAddress, 0x2000000, pattern1, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0);
    if (bResult1 == 0)
        return 0;
    DWORD64 qResult2 = FindPattern(exeBaseAddress, 0x2000000, (BYTE*)&bResult1, "xxxxxxxx", 0);
    DWORD64 *mTarget = (DWORD64*)qResult2;
    DWORD dwProtect1 = NULL;
    DWORD dwProtect2 = NULL;
    VirtualProtect((void*)(&mTarget[1]), sizeof(DWORD64), PAGE_READWRITE, &dwProtect1 );
    VirtualProtect((void*)(&mTarget[5]), sizeof(DWORD64), PAGE_READWRITE, &dwProtect2 );
    mTarget[1] = mTarget[3];
    mTarget[5] = mTarget[3];
    VirtualProtect((void*)(&mTarget[1]), sizeof(DWORD64), dwProtect1, &dwProtect1 );
    VirtualProtect((void*)(&mTarget[5]), sizeof(DWORD64), dwProtect2, &dwProtect2 );
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
