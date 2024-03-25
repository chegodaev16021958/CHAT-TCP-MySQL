#pragma once
#include <string>

using namespace std;

class Message {
private:
	string from;
	string to;
	string data;
public:
	Message():from(""),to(""),data(""){}
	Message(string _from, string _to, string _data):
		from(_from), to(_to), data(_data){}
	void initMessage(string _from, string _to, string _data) {
		from=_from; to=_to; data=_data;
	}
	void setFrom(string _from) {
		from = _from;
	}
	string getFrom() { return from; }
	void setTo(string _to) {
		to = _to;
	}
	string getTo() { return to; }
	void setData(string _data) {
		data = _data;
	}
	string getData() { return data; }
};
