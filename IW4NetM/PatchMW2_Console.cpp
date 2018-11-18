#include "stdafx.h"
#include "hook.h"
#undef MOUSE_MOVED
#include "curses.h"

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

#define MAXPRINTMSG 4096
void Com_Printf(const char *fmt, ...) {
	va_list argptr;
	char msg[MAXPRINTMSG];

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	Sys_Print(msg);
}


DWORD WINAPI ConsoleThread(LPVOID p)
{
	((int(__cdecl*)())0x5CB500)();  //Sys_ShowConsole

	MSG m;

	while (GetMessage(&m, 0, 0, 0)) {
		TranslateMessage(&m);
		DispatchMessage(&m);
	}

	return static_cast<DWORD>(m.lParam);
}

HOOK_DETOUR_DECLARE(hkConsoleInput);

LRESULT __stdcall hkConsoleInput(HWND hWnd, UINT Msg, HWND wParam, LPARAM lParam)
{
	return HOOK_DETOUR_GET_ORIG(hkConsoleInput)(hWnd, Msg, wParam, lParam);
}

void PatchMW2_Console()
{
	//Hide the conhost
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	CreateThread(NULL, 0, ConsoleThread, 0, 0, 0);

	HOOK_DETOUR(0x5CB010, hkConsoleInput);
}