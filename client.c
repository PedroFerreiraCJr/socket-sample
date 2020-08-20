#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
#define true 1

/*
	compile: gcc -Wall -o client client.c
*/

int main(int argc, char const *argv[]) {

	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Falha na criação do socket");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
		perror("Endereço inválido ou não suportado");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Houve uma falha na conexão do socket");
		return -1;
	}

	while (true) {
		printf("Digite algo: ");
		fgets(buffer, sizeof(buffer), stdin);

		if (!strcmp(buffer, "/exit\n")) {
			printf("bye!");
			break;
		}

		send(sock, buffer, strlen(buffer), 0);
		memset(buffer, 0, sizeof(buffer));
	}

	return 0;
}
