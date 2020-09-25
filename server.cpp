#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

using namespace std;

/*
	compile: g++ -Wall --std=c++14 -o server server.cpp
*/

int main(int argc, char const *argv[]) {

	const int PORT {8080};

	int server_fd {0}, new_socket {0}, valread {0};
	struct sockaddr_in address;
	int opt {1};
	int addrlen {sizeof(address)};

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		cout << "Falha na criação do socket" << endl;
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		cout << "Erro na configuração do socket usando a função: setsockopt" << endl;
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0) {
		cout << "A alocação de endereço do socket falhou!" << endl;
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		cout << "Houve uma falha na inicialização da escuta do servidor" << endl;
		exit(EXIT_FAILURE);
	}

	if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*)&addrlen))<0) {
		cout << "Houve uma falha na aceitação de uma nova conexão socket" << endl;
		exit(EXIT_FAILURE);
	}

	char buffer[1024] = {0};
	string *input {nullptr};
	do {	
		valread = read(new_socket, buffer, 1024);
		input = new string(buffer);
		
		if (valread == 0 || input->find("\\exit") != string::npos) {
			cout << "terminando... bye!" << endl;
			memset(buffer, 0, sizeof(buffer));
			input->clear();
			delete input;
			break;
		}

		cout << "Mensagem recebida: " << *input << endl;
		memset(buffer, 0, sizeof(buffer));
		input->clear();
		delete input;
	} while (true);

	return 0;
}
