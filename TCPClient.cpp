#include "TCPClient.h"

void TCPClient::start()
{
	int erStat;
	// Winsock initialiozation
	erStat = WSAStartup(MAKEWORD(2, 2), &wsdata);
	if (erStat != 0) {
		cout << "Error Winsock version initialization #" << WSAGetLastError() << endl;
		return;
	}
#ifdef _DEBUG
	cout << "Winsock initialization" << endl;
#endif
	// Socket initialization
	ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "Eror ititialization Socket #" << WSAGetLastError() << endl;
		Exit();
	}
#ifdef _DEBUG
	cout << "Client Socket initialization" << endl;
#endif
	//initialistanion servinfo
	ZeroMemory(&ServInfo, sizeof(ServInfo));
	in_addr ip_to_num{};
	inet_pton(AF_INET, ip.c_str(), &ip_to_num);
	ServInfo.sin_addr = ip_to_num;
	ServInfo.sin_family = AF_INET;
	ServInfo.sin_port = htons(port);
	//Conection to server
	erStat = connect(ClientSocket, (sockaddr*)&ServInfo, sizeof(ServInfo));
	if (erStat != 0) {
		cout << "Error conection to server #" << WSAGetLastError() << endl;
		Exit();
		return;
	}
#ifdef _DEBUG
	cout << "Connecton to server :sucefull" << endl;
#endif
}

void TCPClient::Send_to_Server(string messege)
{
	int packet_size;
	int msg_size = messege.size();
	packet_size = send(ClientSocket, (char*)&msg_size, sizeof(int), 0);
	if (packet_size == SOCKET_ERROR) {
		cout << "Cant't send size messege to server #" << WSAGetLastError() << endl;
		Exit();
		return;
	}
	packet_size = send(ClientSocket, messege.c_str(), msg_size, 0);
	if (packet_size == SOCKET_ERROR) {
		cout << "Cant't send messege to server #" << WSAGetLastError() << endl;
		Exit();
		return;
	}
#ifdef _DEBUG
	cout << "Messege send to server" << endl;
#endif
}

string TCPClient::Response_from_Server()
{
	int pacet_size;
	int msg_size(0);
	string messege = "";
	pacet_size = recv(ClientSocket, (char*)&msg_size, sizeof(int), 0);
	if (pacet_size == SOCKET_ERROR) {
		cout << "Can't receive size message from Server. Error # " << WSAGetLastError() << endl;
		Exit();
		return messege;
	}
	char* unsver = new char[msg_size + 1];
	unsver[msg_size] = '\0';
	pacet_size = recv(ClientSocket, unsver, msg_size, 0);
	messege = unsver;
	delete[] unsver;
	if (pacet_size == SOCKET_ERROR) {
		cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
		Exit();
		return messege;
	}

#ifdef _DEBUG
	cout << "Messege response from Server" << endl;
#endif

	return messege;
}

void TCPClient::Exit()
{
	closesocket(ClientSocket);
	WSACleanup();
}
