#include "Server.h"

void Server::startServer(string namedb)
{
	db->start(namedb);
	tcp->Start();
	while (true) {
		SOCKET newSock = tcp->aception();
		conect_list.push_back(newSock);
		list<SOCKET>::iterator it = conect_list.end();
		--it;
		thread th(&ClientHendler,newSock,it);
	}
}

void Server::ClientHendler(SOCKET newSock, list<SOCKET>::iterator it)
{
	string code_from_client = "";
	while (code_from_client != to_string(StatRequest::Client_ofline)) {
		code_from_client = tcp->Msg_from_client(newSock);
		int op = stoi(code_from_client);
		switch (op) {
		case StatRequest::Client_ofline:
			CloseClient(it);
			break;
		case StatRequest::FillingContainerMessagess:
			LastMessages(newSock);
			break;
		case StatRequest::login:
			loginClient(newSock);
			break;
		case StatRequest::Registration:
			RegistrationClient(newSock);
			break;
		case StatRequest::SendtoAll:
			ReciveNewMassage(newSock);
			break;
		default:
			break;
		}
	}
}

void Server::CloseClient(list<SOCKET>::iterator it)
{
	closesocket(*it);
	conect_list.erase(it);
}

void Server::RegistrationClient(SOCKET newSock)
{
	User newUser;
	vector <string> unsver_to_db;
	string query;
	string unsver=tcp->Msg_from_client(newSock);
	newUser.set_login(unsver);
	query = "SELECT * FROM users WHERE login=" + newUser.get_login();
	unsver_to_db = db->extract_data(query);   //Проверяем логин полученный от клиента на уникальность
	if (unsver_to_db.empty()) {   // Если в базе логина нет то ждем имя и пароль, и добавляем в базу
		tcp->Send("success", newSock);
		unsver = tcp->Msg_from_client(newSock);
		newUser.set_password(unsver);
		unsver = tcp->Msg_from_client(newSock);
		newUser.set_name(unsver);
		query = "INSERT INTO users(id, login, name, password) values(default, '" + newUser.get_login() + "', '" + newUser.get_name() + "', '" + newUser.getpassword() + "')";
		db->request_to_add(query);
	}
	else
		tcp->Send("login_not_uniq", newSock);
}

void Server::loginClient(SOCKET newSock)
{
	string login = tcp->Msg_from_client(newSock);
	string pas = tcp->Msg_from_client(newSock);
	// Полученные логин и пароль проверяем в базе данных
	string query = "SELECT name FROM users WHERE login='"+login+"' AND password='"+pas+"'";
	vector<string> unsver_from_db = db->extract_data(query);
	if (!unsver_from_db.empty()) {
		tcp->Send("success", newSock);
		tcp->Send(unsver_from_db[0], newSock);
	}
	else
		tcp->Send("no correct to enter", newSock);
}

void Server::ReciveNewMassage(SOCKET newSock)
{
	Message newMes;
	newMes.setFrom(tcp->Msg_from_client(newSock));
	newMes.setTo(tcp->Msg_from_client(newSock));
	newMes.setData(tcp->Msg_from_client(newSock));
	string query = "SELECT id, name FROM users WHERE login = " + newMes.getFrom();
	vector <string> response_from_db = db->extract_data(query);
	string id = response_from_db[0];
	if (newMes.getTo() == "all") {
		query = "INSERT INTO all_messages (id, from_id, data) values (default, " + id + ", " + newMes.getData() + ")";
		if (db->request_to_add(query)) {
			tcp->Send("success", newSock);
		}
		else {
			tcp->Send("Error", newSock);
		}
	}
}

void Server::LastMessages(SOCKET newSock)
{
	vector <string> unsver_from_db = db->extract_data("SELECT MAX(id) FROM all_messages");
	int max_id = stoi(unsver_from_db[0]);
	int curent_id;
	if (max_id > 20) {
		curent_id = max_id - 20;
	}
	else
		curent_id = 1;
	string id = to_string(curent_id);  //определили id от которого будем отсчитывать сообщения которые нужно пережать пользователю
	string query = "SELECT name, data FROM users, all_messages WHERE users.id = all_messages.from_id AND all_messages.id >= " + id;
	unsver_from_db = db->extract_data(query);  //Отправляем векор с сообщениями клиенту
	for (auto& i : unsver_from_db) {
		tcp->Send(i, newSock);
	}
	tcp->Send("_END_", newSock);
}
