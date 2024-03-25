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
	// Функция запускает data base, tcp протокол, принимает поделючения клиентов и многопоточно запускает обработчик запросов для кажого клиента.
	void startServer(string namedb);
	// Метод определяет тип запроса от клиента и вызывает соотвествующий метод. Метод передается в новый поток
	void ClientHendler(SOCKET newSock, list<SOCKET>::iterator it);
	void CloseClient(list<SOCKET>::iterator it);  //  При откючении клиента освобождает Сокет и удаляет из списа подклюючений
	void RegistrationClient(SOCKET newSock);   //Регистрация пользователя
	void loginClient(SOCKET newSock);   //Проверка логина и пароля клиента
	void ReciveNewMassage(SOCKET newSock);
	void LastMessages(SOCKET newSock);  //Метод для отпраки клиенту последних 20 сообщений
};
