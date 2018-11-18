#include "stdafx.h"

FS_ListFiles_t FS_ListFiles = (FS_ListFiles_t)FS_ListFiles_Addr;
DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)DB_FindXAssetHeader_Addr;
Dvar_RegisterInt_t Dvar_RegisterInt = (Dvar_RegisterInt_t)Dvar_RegisterInt_Addr;
Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)Cmd_AddCommand_Addr;
GetSteamID_t GetSteamID = (GetSteamID_t)GetSteamID_Addr;
Sys_Print_t Sys_Print = (Sys_Print_t)Sys_Print_Addr;

HWND *ConsoleOutputHWND = (HWND*)0x642D9D4;
HWND *ConsoleInputHWND = (HWND*)0x642D9E0;
