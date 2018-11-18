#include "stdafx.h"
#include <vector>
#include "hook.h"

#include <iostream>
#include <fstream>

bool Writed = false;

static std::vector<std::pair<std::string, std::string>> _localizedStrings;

HOOK_DETOUR_DECLARE(hkDB_FindXAssetHeader);
NOINLINE const char **hkDB_FindXAssetHeader(assetType_t AssetType, const char *key)
{
	if (AssetType == ASSET_TYPE_LOCALIZE) { //Localized Strings
		const char **ret; // eax@1
		ret = (const char **)HOOK_DETOUR_GET_ORIG(hkDB_FindXAssetHeader)(ASSET_TYPE_LOCALIZE, key);
		
		//*ret = "ฮาท๐มห";
		//printf("\"%s\" \"%s\"\n", key, *ret);
		if (strcmp(key, "XBOXLIVE_SERVICENAME") == 0)
		{
			*ret = VERSIONSTRING;
		}

		return ret;
	}

	return HOOK_DETOUR_GET_ORIG(hkDB_FindXAssetHeader)(AssetType, key);
}

void PatchMW2_LocalizedStrings()
{
	HOOK_DETOUR(DB_FindXAssetHeader_Addr, hkDB_FindXAssetHeader);
}
