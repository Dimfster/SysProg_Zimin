#pragma once
#include "pch.h"

#ifdef SYSTEM_LAB2_DLL_EXPORTS
#define SYSTEM_LAB2_DLL_API __declspec(dllexport)
#else
#define SYSTEM_LAB2_DLL_API __declspec(dllimport)
#endif

struct header
{
	int addr;
	//int command;
	int size;
};

extern "C" {
	SYSTEM_LAB2_DLL_API void sendZimin(int addr, const wchar_t* str);
	SYSTEM_LAB2_DLL_API std::wstring receiveZimin(header& h);
}
