#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

/*
	compile: g++ -Wall --std=c++14 -o client client.cpp
*/
int main(int argc, char const *argv[]) {

	const int PORT {8080};

	int sock {0};
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "Falha na criação do socket" << endl;
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
		cout << "Endereço inválido ou não suportado" << endl;
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		cout << "Houve uma falha na conexão do socket" << endl;
		return -1;
	}

	string input {0};
	while (true) {
		cout << "Digite algo: " << endl;
		getline(cin, input);

		if (input.find("\\exit") != string::npos) {
			cout << "bye!" << endl;
			send(sock, input.c_str(), input.length(), 0);
			input.clear();
			break;
		}

		send(sock, input.c_str(), input.length(), 0);
		input.clear();
	}

	return 0;
}
