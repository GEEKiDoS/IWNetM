#include "stdafx.h"
#include <stdio.h>
#include <fstream>
#include <io.h>  
#include <fcntl.h>
#include "hook.h"

//Thanks to Nukem
bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;

	return (*szMask) == NULL;
}

//Thanks to Nukem
DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char *szMask)
{
	dwLen += dwAddress;

	for (DWORD i = dwAddress; i < dwLen; i++)
		if (Compare((BYTE *)i, bMask, szMask))
		{
			return i;
		}

	return 0;
}

HOOK_DETOUR_DECLARE(hkGetName);
char name[255] = "^1I Fucked Up";

NOINLINE const char* __fastcall hkGetName()
{
	return name;
}

HOOK_DETOUR_DECLARE(hkMatchMakingInfo);

NOINLINE void __cdecl hkMatchMakingInfo()
{
	*(DWORD*)0x66421F8 = (DWORD)Dvar_RegisterInt("ping_searches_per", 2, 1, 30, DVAR_FLAG_NONE, "Number of searches at each min ping value");
	*(DWORD*)0x663F1AC = (DWORD)Dvar_RegisterInt("ping_increment", 10, 1, 500, DVAR_FLAG_NONE, "Number of milliseconds to increase min ping after each set of searches");
	*(DWORD*)0x6644390 = (DWORD)Dvar_RegisterInt("ping_default_min", 500, 10, 500, DVAR_FLAG_NONE, "Minimum ping for the initial set of searches");
	*(DWORD*)0x6641350 = (DWORD)Dvar_RegisterInt("session_advertiseFailedSearchCount", 6, 0, 500, DVAR_FLAG_NONE, "Failed search count at which we list ourselves with matchmaking");
}

void printSteamID_f()
{
	__int64 steamid = GetSteamID();
	Com_Printf("%llx\n", steamid);
}

void PatchMW2_ColorTable();
void PatchMW2_Console();
void PatchMW2_NoBorder();
void PatchMW2_LocalizedStrings();

void Sys_RunInit()
{
	PatchMW2_Console();
	PatchMW2_ColorTable();
	PatchMW2_NoBorder();
	PatchMW2_LocalizedStrings();

	GetPrivateProfileStringA("IWNet", "Name", "^6Unknown Soilder", name, 255, ".\\IWNet.ini");

	// Set Name
	HOOK_DETOUR(0x627500, hkGetName);

	// Speed up the matchmaking
	HOOK_DETOUR(0x621660, hkMatchMakingInfo);

	// FS_BaseGame *NOT WORKING
	*(DWORD*)0x6431D1 = (DWORD)"IWNetM";

	// Window Title
	*(DWORD*)0x418B46 = (DWORD)"Call of Duty: Modern Warfare 2 - IWNetM";

	*(DWORD*)0x5CB352 = (DWORD)"Consolas";

	// cg_scoreboardPingText
	*(BYTE*)0x4A4CCE = 1;

	// Remove exec whitelist
	memset((void*)0x564A05, 0x90, 5);
	*(WORD*)0x564A0C = 0x9090;

	// Add quit back, Why they deleted quit command in 1.2.211?
	static cmd_function_t quitCmd;
	Cmd_AddCommand("quit", ((void(*)())0x56A2D0), &quitCmd);

	static cmd_function_t steamidCmd;
	Cmd_AddCommand("steamid", printSteamID_f, &steamidCmd);
}

DWORD* color_table;

DWORD WINAPI RGBColor(LPVOID p) // R!!!G!!!B!!!
{
	int i = 1, redVal = 255, greenVal = 1, blueVal = 1;

	while (true)
	{
		i += 1;
		if (i < 255)
		{
			redVal -= 1;
			greenVal += 1;
			blueVal = 1;
		}
		else if (i < 509)
		{
			redVal = 1;
			greenVal -= 1;
			blueVal += 1;
		}
		else if (i < 763)
		{
			redVal += 1;
			greenVal = 1;
			blueVal -= 1;
		}
		else
		{
			i = 1;
		}

		color_table[6] = RGB(redVal, greenVal, blueVal);

		Sleep(14);
	}

	return 0;
}

void PatchMW2_ColorTable()
{
	color_table = (DWORD*)FindPattern(0x401000, 0x800000, (PBYTE)"\x00\x00\x00\xFF\xFF\x5C\x5C\xFF\x00\xFF\x00\xFF\xFF\xFF\x00\xFF\x00\x00\xFF\xFF\x00\xFF\xFF\xFF\xFF\x5C\xFF\xFF\xFF\xFF\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	color_table[1] = RGB(255, 49, 49);
	color_table[2] = RGB(134, 192, 0);
	color_table[3] = RGB(255, 173, 34);
	color_table[4] = RGB(0, 135, 193);
	color_table[5] = RGB(32, 197, 255);
	color_table[6] = RGB(255, 1, 1);

	CreateThread(NULL, 0, RGBColor, 0, 0, 0);
}

