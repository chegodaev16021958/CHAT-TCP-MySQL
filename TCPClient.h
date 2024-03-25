#pragma once
#include <string>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

constexpr int BUFSIZE = 1024;

class TCPClient {
private:
	string ip;
	int port;
	SOCKET ClientSocket;
	sockaddr_in ServInfo;
	WSADATA wsdata;
public:
	TCPClient(string ip_addr, int _port) :
		ip(ip_addr), port(_port) {}
	void start();
	void Send_to_Server(string messege);
	string Response_from_Server();
	void Exit();
};
