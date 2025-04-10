#include "Session.h"

Session::Session(int sessionID)
    : sessionID(sessionID)
{
    //InitializeCriticalSection(&cs);
    std::lock_guard<std::mutex> lock(mtx); 
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

Session::~Session()
{
    //DeleteCriticalSection(&cs);
    CloseHandle(hEvent);
}

void Session::addMessage(Message& m)
{
    //EnterCriticalSection(&cs);
    std::lock_guard<std::mutex> lock(mtx); 
    messages.push(m);
    SetEvent(hEvent);
    //LeaveCriticalSection(&cs);
}

bool Session::getMessage(Message& m)
{
    bool res = false;
    WaitForSingleObject(hEvent, INFINITE);
    //EnterCriticalSection(&cs);
    std::lock_guard<std::mutex> lock(mtx); 
    if (!messages.empty())
    {
        res = true;
        m = messages.front();
        messages.pop();
    }
    if (messages.empty())
    {
        ResetEvent(hEvent);
    }
    //LeaveCriticalSection(&cs);
    return res;
}

void Session::addMessage(MessageTypes messageType, const wstring& data)
{
    Message m(messageType, data);
    addMessage(m);
}
