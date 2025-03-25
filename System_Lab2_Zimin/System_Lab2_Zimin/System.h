#pragma once

#include <windows.h>
#include <conio.h>

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>


using namespace std;


// Функция для преобразования string -> wstring
inline wstring StringToWString(const string& str)
{
    return wstring(str.begin(), str.end());
}

inline void DoWrite()
{
    wcout << endl;
}

template <class T, typename... Args>
inline void DoWrite(T& value, Args... args)
{
    if constexpr (is_same_v<T, string>)  
        wcout << StringToWString(value) << L" ";
    else
        wcout << value << L" ";  

    DoWrite(args...);
}

static mutex m;
//static CRITICAL_SECTION cs;
//static bool initCS = true;

template <typename... Args>
inline void SafeWrite(Args... args)
{
    //if (initCS)
    //{
    //    InitializeCriticalSection(&cs);
    //    initCS = false;
    //}
    //EnterCriticalSection(&cs);
    lock_guard<mutex> lg(m);
    DoWrite(args...);
    //LeaveCriticalSection(&cs);
}


inline void WriteToFile(int num, const wstring& message)
{
    string filename = to_string(num) + ".txt";

    wofstream file(filename, ios::trunc);

    file.imbue(locale("ru_RU.utf8"));

    if (file.is_open()) {
        file << message << endl;
    }

}


