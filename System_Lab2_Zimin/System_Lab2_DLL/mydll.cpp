#include "pch.h"
#include "mydll.h"

HANDLE Zimin_Mutex = CreateMutexW(NULL, FALSE, L"ZIMIN_MUTEX");


void sendZimin(int addr, const wchar_t* str) {
	WaitForSingleObject(Zimin_Mutex, INFINITE);
	header h = { addr, int(wcslen(str) * 2) };
	HANDLE hFile = CreateFileW(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);

	HANDLE hFileMap = CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, h.size + sizeof(header), L"MyMap");
	BYTE* buff = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.size + sizeof(header));

	memcpy(buff, &h, sizeof(header));
	memcpy(buff + sizeof(header), str, h.size);

	UnmapViewOfFile(buff);

	CloseHandle(hFileMap);
	CloseHandle(hFile);

	ReleaseMutex(Zimin_Mutex);
}

std::wstring receiveZimin(header& h) {
	WaitForSingleObject(Zimin_Mutex, INFINITE);
	HANDLE hFile = CreateFileW(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);


	HANDLE hFileMap = CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, sizeof(header), L"MyMap");

	LPVOID buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));
	h = *((header*)buff);

	UnmapViewOfFile(buff);
	CloseHandle(hFileMap);

	int n = h.size + sizeof(header);
	hFileMap = CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, n, L"MyMap");
	buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, n);
	std::wstring s((wchar_t*)((BYTE*)buff + sizeof(header)), h.size / 2);

	UnmapViewOfFile(buff);
	CloseHandle(hFileMap);
	CloseHandle(hFile);

	ReleaseMutex(Zimin_Mutex);
	return s;
}