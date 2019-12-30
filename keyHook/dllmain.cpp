// dllmain.cpp : Defines the entry point for the DLL application.
#include <string>
#include <algorithm>
#include "stdafx.h"
#include "keyHook.h"


HMODULE hInst;

std::string GetCurrentProcssName(HMODULE hDllModule)
{
	TCHAR exeFullPath[MAX_PATH];
	std::string strPath = _T("");

	GetModuleFileName(hDllModule,exeFullPath,MAX_PATH);
	strPath=(std::string)exeFullPath;
	int pos = strPath.find_last_of('\\', strPath.length());
	return strPath.substr(pos+1);
}

std::string StringToLow(std::string& strSource)
{
	std::transform(strSource.begin(), strSource.end(), strSource.begin(), tolower);

	return strSource;
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	//TODO:  Check the current process name is explorer.exe or not. 
	case DLL_PROCESS_ATTACH:
		hInst = hModule;
		{
			std::string process_name = GetCurrentProcssName(NULL);
			std::string low_process_name = StringToLow(process_name);
			if (low_process_name == _T("explorer.exe"))
			{
				DebugOutputMsg(_T("Dllmain ATTTACH, process_name = %s."), process_name.c_str());
				SetKbHook();
			}
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		std::string process_name = GetCurrentProcssName(NULL);
		std::string low_process_name = StringToLow(process_name);
		if (low_process_name == _T("explorer.exe"))
		{
			DebugOutputMsg(_T("Dllmain DETACH, process_name = %s."), process_name.c_str());
			RemoveKbHook();
		}
		break;
	}
	return TRUE;
}


