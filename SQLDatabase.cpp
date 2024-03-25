#include "SQLDatabase.h"

void DataBase::start(string namedb)
{
	if (!mysql_real_connect(&mysql, "localhost", "denis", "2003", namedb.c_str(), NULL, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	mysql_query(&mysql, "create table users(id int auto_increment primary key, login varchar(255) not null, name varchar(255), password varchar(255) not null)");
	mysql_query(&mysql, "create table messages(id int auto_increment primary key, from int references users(id), data varchar(255), to int references user(id))");
	mysql_query(&mysql, "create table all_messages(id int auto_increment primary key, from_id int references users(id), data varchar(255))");
}

bool DataBase::request_to_add(string request)
{
	bool res = false;
	if (mysql_query(&mysql, request.c_str())) {
		cout << "Error: " << mysql_error(&mysql) << endl;
	}
	else {
		res = true;
	}
	return res;
}

vector<string> DataBase::extract_data(string request)
{
	vector<string> unsver;
	MYSQL_RES* res;
	MYSQL_ROW row;
	mysql_query(&mysql, request.c_str());
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (unsigned int i = 0; i < mysql_num_fields(res); i++) {
				unsver.push_back(row[i]);
			}
		}
	}
	else
		std::cout << "Error request: " << mysql_error(&mysql) << endl;
	return unsver;
}

void DataBase::stop()
{
	mysql_close(&mysql);
}