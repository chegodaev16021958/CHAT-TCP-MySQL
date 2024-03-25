#pragma once
#include <thread>
#include <list>
#include "TcpServer.h"
#include"../../src/User.h"
#include"../../src/Message.h"
#include"../../src/StatRequest.h"
#include "SQLDatabase.h"

using namespace std;

class Server {
private:
	DataBase* db;
	TCPServer* tcp;
	list<SOCKET> conect_list;
public:
	Server(string ip, int port) {
		tcp = new TCPServer(ip, port);
		db = new DataBase();
	}
	~Server() {
		delete db;
		delete tcp;
	}
	// ������� ��������� data base, tcp ��������, ��������� ����������� �������� � ������������ ��������� ���������� �������� ��� ������ �������.
	void startServer(string namedb);
	// ����� ���������� ��� ������� �� ������� � �������� �������������� �����. ����� ���������� � ����� �����
	void ClientHendler(SOCKET newSock, list<SOCKET>::iterator it);
	void CloseClient(list<SOCKET>::iterator it);  //  ��� ��������� ������� ����������� ����� � ������� �� ����� ������������
	void RegistrationClient(SOCKET newSock);   //����������� ������������
	void loginClient(SOCKET newSock);   //�������� ������ � ������ �������
	void ReciveNewMassage(SOCKET newSock);
	void LastMessages(SOCKET newSock);  //����� ��� ������� ������� ��������� 20 ���������
};
