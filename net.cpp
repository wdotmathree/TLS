#include "net.hpp"

int sock_connect(const char *host, const int port) {
	int sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	struct addrinfo *res;
	if (getaddrinfo(host, NULL, NULL, &res) != 0) {
		perror("getaddrinfo");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("connect");
		return -1;
	}

	return sock;
}
