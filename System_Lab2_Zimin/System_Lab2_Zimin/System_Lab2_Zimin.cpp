#include "System.h"
#include "Session.h"
#include "mydll.h"

void MyThread(Session* session)
{
	SafeWrite("session", session->sessionID, "created");
	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				SafeWrite("session", session->sessionID, "closed");
				delete session;
				return;
			}
			case MT_DATA:
			{
				SafeWrite("session", session->sessionID, "data", m.data);
				WriteToFile(session->sessionID, m.data);
				break;
			}
			}
		}
	}
	return;
}

int main()
{
#ifdef _WIN32
	wcin.imbue(locale("rus_rus.866"));
	wcout.imbue(locale("rus_rus.866"));
#else
	std::wcin.imbue(std::locale("ru_RU.UTF-8"));
	std::wcout.imbue(std::locale("ru_RU.UTF-8"));
#endif

	vector<Session*> sessions;
	int i = 1;

	HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, L"StartEvent");
	HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, L"StopEvent");
	HANDLE hTransportEvent = CreateEvent(NULL, FALSE, FALSE, L"TransportEvent");
	HANDLE hExitEvent = CreateEvent(NULL, FALSE, FALSE, L"ExitEvent");
	HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, L"ConfirmEvent");

	HANDLE hControlEvents[4] = { hStartEvent, hStopEvent, hTransportEvent, hExitEvent };

	//sendZimin(1, L"Ljynajjynyjmnjym"); // ??????????????????????
	do
	{
		int n = WaitForMultipleObjects(4, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
		switch (n)
		{
			case 0: 
			{
				sessions.push_back(new Session(i++));
				thread t(MyThread, sessions.back());
				t.detach();
				//CloseHandle(CreateThread(NULL, 0, MyThread, (LPVOID)sessions.back(), 0, NULL));
				SetEvent(hConfirmEvent);
				break;
			}
			case 1:
			{ 
				--i;
				if (i != 0) {
					sessions[i - 1]->addMessage(MT_CLOSE);
					sessions.pop_back();
					SetEvent(hConfirmEvent);
				}
				else {
					SetEvent(hConfirmEvent);
				}

				break;
			}
			case 2:
			{
				header h;
				wstring str = receiveZimin(h);

				switch (h.addr) 
				{
				case -2:
				{
					SafeWrite(L"Main thread data", str);
					for (auto session : sessions) {
						session->addMessage(MT_DATA, str);
					}
					break;
				}
				case -1:
				{
					SafeWrite(L"Main thread data", str);
					break;
				}
				default:
				{
					sessions[h.addr]->addMessage(MT_DATA, str);
					break;
				}
				}

				SetEvent(hConfirmEvent);
				break;
			}

			case 3:
			{
				SetEvent(hConfirmEvent);
				return 0;
			}
		}
	} while (i);

	SetEvent(hConfirmEvent);

	return 0;
}

