// dllmain.cpp : Defines the entry point for the DLL application.
#include <string>
#include <algorithm>
#include "stdafx.h"
#include "keyHook.h"


HMODULE hInst;

// get current process name, without directories
void GetCurrentProcssName(HMODULE hDllModule, char *process_name)
{
	char exeFullPath[MAX_PATH];

	GetModuleFileName(hDllModule,exeFullPath,MAX_PATH);
	char *slash = strrchr(exeFullPath, '\\');
	if (slash) 
	{
		strcpy(process_name, slash + 1);
	}
	else 
	{
		strcpy(process_name, exeFullPath);
	}
}

// check if the process loading this dll is our target process (explorer.exe)
bool CurrentProcessIsTargetProcess(const char * targetProcessName) {
	char process_name[MAX_PATH];
	GetCurrentProcssName(NULL, process_name);
	_strlwr(process_name);
	if (strcmp(process_name, targetProcessName)==0)
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
			// set the 
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


