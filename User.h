#pragma once
#include <string>
using namespace std;

class User {
	string name;
	string login;
	string password;
public:
	User():name(""),login(""),password(""){}
	User(string _name, string _login, string _password):
		name(_name), login(_login), password(_password){}
	void initUser(string _name, string _login, string _password) {
		name = _name; login = _login; password = _password;
	}
	void set_name(string newname) {
		name = newname;
	}
	string get_name() { return name; }
	void set_login(string newlogin) {
		login = newlogin;
	}
	string get_login() { return login; }
	void set_password(string newpas) {
		password = newpas;
	}
	string getpassword() { return password; }
};
