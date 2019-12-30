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

// 
bool CurrentProcessIsTargetProcess(std::string targetProcessName) {
	std::string process_name = GetCurrentProcssName(NULL);
	std::string low_process_name = StringToLow(process_name);
	if (low_process_name == targetProcessName)
	{
		return true;
	}

	return false;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// DLL is being mapped into the process' address space.
		hInst = hModule;
		//TODO:  Check the current process name is explorer.exe or not, if true set keyboard hook.
		if (CurrentProcessIsTargetProcess(_T("explorer.exe")))
		{
			DebugOutputMsg(_T("DLL_PROCESS_ATTACH Set Key borard hook."));
			SetKbHook();
		}
		break;
	case DLL_THREAD_ATTACH:
		// A thread is being created.
		break;
	case DLL_THREAD_DETACH:
		// A thread is exiting cleanly.
		break;
	case DLL_PROCESS_DETACH:
		// DLL is being unmapped from the process' address space.
		//TODO:  Check the current process name is explorer.exe or not, if true remove keyboard hook.
		if (CurrentProcessIsTargetProcess(_T("explorer.exe")))
		{
			DebugOutputMsg(_T("DLL_PROCESS_DETACH remove Key borard hook."));
			RemoveKbHook();
		}
		break;
	}
	return TRUE;
}


