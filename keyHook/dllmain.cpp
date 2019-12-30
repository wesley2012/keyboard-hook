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

	// TO DO: use GetModuleFileNameA to get full path of current process 
	// for GetModuleFileNameA see https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
	
	// TO DO: extract the file name without directories, store it into process_name
	
}

// check if the process loading this dll is our target process (explorer.exe)
bool CurrentProcessIsTargetProcess(const char * targetProcessName) {
	char process_name[MAX_PATH];
	GetCurrentProcssName(NULL, process_name);
	_strlwr(process_name); // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strlwr-wcslwr-mbslwr-strlwr-l-wcslwr-l-mbslwr-l?view=vs-2019
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
		//       You may need the function CurrentProcessIsTargetProcess
		
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
		//       You may need the function CurrentProcessIsTargetProcess
		
		break;
	}
	return TRUE;
}


