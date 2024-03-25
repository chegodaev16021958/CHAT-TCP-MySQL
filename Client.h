#pragma once
#include <vector>
#include "../../src/Message.h"
#include "../../src/User.h"
#include"TCPClient.h"
#include "../../src/StatRequest.h"
#include<list>

using namespace std;

class Client {
private:
	list <Message> Messages;
	User *curentUser;
	TCPClient *tcp;
	bool activate;

public:
	Client(string ip, int port) {
		curentUser = new User();
		tcp = new TCPClient(ip,port);
		activate = false;
	}
	~Client() {
		delete tcp;
		delete curentUser;

	}
	void start(string ip, int port);
	void registration();
	void login();
	void FillingContainer();
	void chat();
	void PrintMesseges();
	void SendMessege();
	//void stop();
};
