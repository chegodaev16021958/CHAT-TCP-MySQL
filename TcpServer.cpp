#include "TcpServer.h"

void TCPServer::Start()
{
	int ErStat(0);
	ErStat = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (ErStat != 0) {
		cout << "Error Winsock version initialization #" << WSAGetLastError() << endl;
		return;
	}
#ifdef _DEBUG
	cout << "Winsock initialization" << endl;
#endif
	// Socket initialization
	ServSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServSocket == INVALID_SOCKET) {
		cout << "Eror ititialization Socket #" << WSAGetLastError() << endl;
		Exit();
	}
#ifdef _DEBUG
	cout << "Client Socket initialization" << endl;
#endif
	//Sockaddr inittialization
	ZeroMemory(&servaddress, sizeof(servaddress));
	in_addr ip_to_num{};
	inet_pton(AF_INET, addres_serv.c_str(), &ip_to_num);
	servaddress.sin_addr = ip_to_num;
	servaddress.sin_family = AF_INET;
	servaddress.sin_port = htons(port);
	//bind
	ErStat = bind(ServSocket, (sockaddr*)&servaddress, sizeof(servaddress));
	if (ErStat != 0) {
		cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << endl;
		Exit();
		return;
	}
#ifdef _DEBUG
	cout << "Serv socket is successfully bound" << endl;
#endif
	ErStat = listen(ServSocket, SOMAXCONN);

	if (ErStat != 0) {
		cout << "Can't start to listen to. Error # " << WSAGetLastError() << endl;
		Exit();
		return;
	}
#ifdef _DEBUG
	cout << "Listening to port" << endl;
#endif
}

void TCPServer::Send(string messege, SOCKET newConect)
{
	int packet_size;
	int msg_size = messege.size();
	packet_size = send(newConect, (char*)&msg_size, sizeof(int), 0);
	if (packet_size == SOCKET_ERROR) {
		cout << "Cant't send size messege to client #" << WSAGetLastError() << endl;
		Exit();
		return;
	}
	packet_size = send(newConect, messege.c_str(), msg_size, 0);
	if (packet_size == SOCKET_ERROR) {
		cout << "Cant't send messege to Client #" << WSAGetLastError() << endl;
		Exit();
		return;
	}
#ifdef _DEBUG
	cout << "Messege send to Client" << endl;
#endif
}

string TCPServer::Msg_from_client(SOCKET newConect)
{
	int pacet_size;
	int msg_size(0);
	string messege = "";
	pacet_size = recv(newConect, (char*)&msg_size, sizeof(int), 0);
	if (pacet_size == SOCKET_ERROR) {
		cout << "Can't receive size message from Client. Error # " << WSAGetLastError() << endl;
		Exit();
		return messege;
	}
	char* unsver = new char[msg_size + 1];
	unsver[msg_size] = '\0';
	pacet_size = recv(newConect, unsver, msg_size, 0);
	messege = unsver;
	delete[] unsver;
	if (pacet_size == SOCKET_ERROR) {
		cout << "Can't receive message from Client. Error # " << WSAGetLastError() << endl;
		Exit();
		return messege;
	}

#ifdef _DEBUG
	cout << "Messege response from client" << endl;
#endif

	return messege;
}

SOCKET TCPServer::aception()
{
	sockaddr_in Client_Info;
	ZeroMemory(&Client_Info, sizeof(Client_Info));
	int clienSize = sizeof(Client_Info);
	SOCKET newCon = accept(ServSocket, (sockaddr*)&Client_Info, &clienSize);
	if (newCon == INVALID_SOCKET) {
		cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << endl;
		closesocket(newCon);
	}
	return newCon;
}

void TCPServer::Exit()
{
	closesocket(ServSocket);
	WSACleanup();
}