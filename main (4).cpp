#include"Client.h"

int main() {
	Client client("127.0.0.1", 45000);
	client.start("127.0.0.1", 45000);
	return 0;
}