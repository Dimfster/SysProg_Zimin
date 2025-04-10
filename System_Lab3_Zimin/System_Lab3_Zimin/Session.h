#pragma once

#include "System.h"

enum MessageTypes
{
	MT_CLOSE,
	MT_DATA
};

struct MessageHeader
{
	int messageType;
	int size;
};

struct Message
{
	MessageHeader header = { 0 };
	wstring data;
	Message() = default;
	Message(MessageTypes messageType, const wstring& data = L"")
		:data(data)
	{
		header = { messageType,  int(data.length()) };
	}
};


class Session
{
private:
	queue<Message> messages;
	mutex mtx;
	HANDLE hEvent;

public:
	int sessionID;

	Session(int sessionID);
	~Session();

	void addMessage(Message& m);
	bool getMessage(Message& m);
	void addMessage(MessageTypes messageType, const wstring& data = L"");
};