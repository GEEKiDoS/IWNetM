#include "stdafx.h"
#include "hook.h"
#include "Hooking.h"

StompHook windowedWindowStyleHook;
DWORD windowedWindowStyleHookLoc = 0x418AE9;

bool IsNoborder = true;

void __declspec(naked) WindowedWindowStyleHookStub()
{
	if (IsNoborder)
	{
		__asm mov ebp, WS_VISIBLE | WS_POPUP

		WritePointer(0x695D9EC, 0xC, 0);
		WritePointer(0x695D940, 0xC, 0);
	}
	else
	{
		__asm mov ebp, WS_VISIBLE | WS_SYSMENU | WS_CAPTION
	}

	__asm retn
}

void PatchMW2_NoBorder()
{
	int noborder = GetPrivateProfileInt("IWNet", "NoBorder", 1, ".\\IWNet.ini");
	IsNoborder = noborder == 0 ? false : true;

	windowedWindowStyleHook.initialize(windowedWindowStyleHookLoc, WindowedWindowStyleHookStub, 5, false);
	windowedWindowStyleHook.installHook();
}