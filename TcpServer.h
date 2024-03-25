#pragma once
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;



class TCPServer {
private:
	WSADATA wsadata;
	SOCKET ServSocket;
	string addres_serv;
	int port;
	sockaddr_in servaddress;
public:
	TCPServer(string address, int _port) :
		addres_serv(address), port(_port) {}
	void Start();
	void Send(string mesg, SOCKET newConect);
	string Msg_from_client(SOCKET newConect);
	SOCKET aception();
	void Exit();
};