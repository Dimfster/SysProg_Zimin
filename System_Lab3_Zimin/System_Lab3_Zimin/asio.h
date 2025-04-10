#pragma once

#ifndef _WIN32_WINNT
#define	_WIN32_WINNT	0x0A00
#endif						

#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;


struct command
{
	int addr;
	int command;
	wstring message;
};

inline int receiveInt(tcp::socket& s)
{
	int n;
	boost::system::error_code error;
	boost::asio::read(s, boost::asio::buffer(&n, sizeof(n)), error);
	if (error)
		throw boost::system::system_error(error);
	return n;
}

inline void sendInt(tcp::socket& s, int n)
{
	boost::system::error_code error;
	boost::asio::write(s, boost::asio::buffer(&n, sizeof(n)), error);
	if (error)
		throw boost::system::system_error(error);
}

inline wstring receiveString(tcp::socket& s)
{
	int n = receiveInt(s);
	wstring str;
	str.resize(n / sizeof(wchar_t));

	boost::system::error_code error;
	boost::asio::read(s, boost::asio::buffer(str.data(), n), error);
	if (error)
		throw boost::system::system_error(error);

	//h = *((header*)buff);

	return str;
}

inline void sendString(tcp::socket& s, const wstring& str)
{
	int n = int(str.length() * sizeof(wchar_t));
	sendInt(s, n);

	boost::system::error_code error;
	boost::asio::write(s, boost::asio::buffer(str.data(), n), error);
	if (error)
		throw boost::system::system_error(error);
}



inline void receiveCommand(tcp::socket& s, command& c)
{
	c.command = receiveInt(s);
	if (c.command == 2){
		c.addr = receiveInt(s);
		c.message = receiveString(s);
	}
	return;
}

