#pragma once
#include "mysql.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class DataBase {
private:
	MYSQL mysql;
public:
	DataBase() {
		mysql_init(&mysql);
	}
	void start(string namedb);
	bool request_to_add(string request);
	vector<string> extract_data(string request);
	void stop();
};