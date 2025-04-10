//#include "mydll.h"
#include "asio.h"
#include "Session.h"

vector<Session*> sessions;
vector<shared_ptr<tcp::socket>> clients;
mutex clientsMutex; 
int numSessions = 0;

void MyThread(Session* session)
{
	SafeWrite("Session", session->sessionID, "created");
	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				SafeWrite("Session", session->sessionID, "closed");
				delete session;
				return;
			}
			case MT_DATA:
			{
				SafeWrite("Session", session->sessionID, "message", m.data);
				WriteToFile(session->sessionID, m.data);
				break;
			}
			}
		}
	}
	return;
}

void launchClient()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	wchar_t path[] = L"System_Lab2_CSharp.exe";
	CreateProcess(NULL, path, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void processClient(tcp::socket s)
{
	try
	{
		while (true){
			command c;
			receiveCommand(s, c);
			switch (c.command)
			{
			case 0:
			{
				sessions.push_back(new Session(numSessions++));
				thread t(MyThread, sessions.back());
				t.detach();
				sendInt(s, numSessions);
				break;
			}
			case 1:
			{
				if (numSessions != 0) {
					--numSessions;
					sessions[numSessions]->addMessage(MT_CLOSE);
					sessions.pop_back();
					sendInt(s, numSessions);
				}
				else {
					sendInt(s, -1);
				}
				break;
			}
			case 2:
			{
				switch (c.addr)
				{
				case -2:
				{
					SafeWrite(L"Main thread message", c.message);
					for (auto session : sessions) {
						session->addMessage(MT_DATA, c.message);
					}
					break;
				}
				case -1:
				{
					SafeWrite(L"Main thread message", c.message);
					break;
				}
				default:
				{
					sessions[c.addr]->addMessage(MT_DATA, c.message);
					break;
				}
				}
				sendInt(s, numSessions);
				break;
			}
			case 3:
			{
				SafeWrite("Client left the server");
				sendString(s, L"ok");
				return;
			}
			case 4:
			{
				sendInt(s, numSessions);
				break;
			}
			}
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Client exception: " << e.what() << endl;
	}
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

	try
	{
		int port = 5284;
		boost::asio::io_context io;
		tcp::acceptor a(io, tcp::endpoint(tcp::v4(), port));

		for (int i = 0; i < 2; i++) {
			launchClient();
		}
		
		while (true)
		{
			std::thread(processClient, a.accept()).detach();
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Exception: " << e.what() << endl;
	}
	return 0;
}

