#include "Client.h"

void Client::start(string ip, int port)
{
	tcp = new TCPClient(ip, port);
	tcp->start();
	int choice = 1;

	while (choice) {
		cout << "Hello, please enter:" << endl << "1. Registration" << endl << "2. Authorization" << endl << "0. Exit" << endl;
		cin >> choice;
		switch (choice) {
		case 1:
			registration();
			break;
		case 2:
			login();
			break;
		case 0:
			cout << "Goodbye" << endl;
			break;
		default:
			cout << "The item you entered is missing from the menu" << endl;
			break;
		}
	}
}

void Client::registration()
{
	string login = "";
	string pas = "";
	string name = "";
	cout << "please enter login. Enter q to exit" << endl;
	cin >> login;
	if (login == "q")
		return;
	while (login == "") {
		cout << "The login cannot be empty! please repeat!" << endl;
		cin >> login;
	}
	cout << "please enter password. Enter q to exit" << endl;
	cin >> pas;
	if (pas == "q")
		return;
	while (pas == "") {
		cout << "Password cannot be emty! please repeat!" << endl;
		cin >> pas;
	}
	cout << "please enter tour name. Enter q to exit" << endl;
	cin >> name;
	if (name == "q")
		return;
	while (name == "") {
		cout << "name cannot be emty! please repeat!" << endl;
		cin >> name;
	}
	
	string unsver = "";
	do
	{
		tcp->Send_to_Server(to_string(StatRequest::Registration));
		tcp->Send_to_Server(login);
		unsver = tcp->Response_from_Server();
		if (unsver == "success") {
			cout << "Registration is succeful!" << endl;
			tcp->Send_to_Server(pas);
			tcp->Send_to_Server(name);
		}
		else {
			cout << "the login is alredy in use. please enter login repeat." << endl;
			cin >> login;

		}
	} while (unsver!="success");
}

void Client::login()
{
	if (!activate) {
		string login = "";
		string pas = "";
		string name = "";
		string unsver="";
		do
		{
			cout << "please Enter your login. Enter q to exit." << endl;
			cin >> login;
			if (login == "q") {
				return;
			}
			cout << "please enter your password.Enter q to exit." << endl;
			cin >> pas;
			if (pas == "q")
				return;
			tcp->Send_to_Server(to_string(StatRequest::login));
			tcp->Send_to_Server(login);
			tcp->Send_to_Server(pas);
			unsver = tcp->Response_from_Server();
			if (unsver == "success") {  //≈сли успех ждем от сервера им€, заолн€ем User и переходим в чат
				name = tcp->Response_from_Server();
				curentUser->set_name(name);
				curentUser->set_login(login);
				curentUser->set_password(pas);
				activate = true;
				FillingContainer();
				cout << "         ===Hello " << curentUser->get_name() << "===" << endl;
				chat();
			}
			else
				cout << "login or password is not correct.please repeat." << endl;
		} while (unsver!="success");
	}
	else {
		cout << "         ===Hello " << curentUser->get_name() << "===" << endl;
		chat();
	}
}

void Client::FillingContainer()
{
	tcp->Send_to_Server(to_string(StatRequest::FillingContainerMessagess));
	while (true) {
		Message msg;
		msg.setFrom(tcp->Response_from_Server());
		if (msg.getFrom() == "_END_")    //END прийдет, когда с сервера пийдут все сообщени€
			break;
		msg.setData(tcp->Response_from_Server());
		msg.setTo("all");
		Messages.push_back(msg);
	}
}

void Client::chat()
{
	PrintMesseges();
	int choice = 1;
	while (choice) {
		cin >> choice;
		switch (choice)
		{
		case 1:
			SendMessege();
			break;
		default:
			break;
		}
	}
}

void Client::PrintMesseges()
{
	system("cls");
	for (auto &i : Messages) {
		cout << i.getFrom() << ":  " << i.getData() << endl;
	}
	cout << "Enter: 1 - Send to messege, 0 - Exit." << endl;
}

void Client::SendMessege()
{
	tcp->Send_to_Server(to_string(StatRequest::SendtoAll));
	string msg = "";
	string to = "";
	cout << "who do you want to send a message to? if all enter: all" << endl;
	cin >> to;
	cout << "Enter to message" << endl;
	cin >> msg;
	tcp->Send_to_Server(curentUser->get_login());
	tcp->Send_to_Server(to);
	tcp->Send_to_Server(msg);
	string unsver = tcp->Response_from_Server();
	if (unsver == "success") {
		cout << "Message send to server" << endl;
	}
	else
		cout << "Error sending to message" << endl;
}


