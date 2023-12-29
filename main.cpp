#include "net.hpp"

int do_handshake(int sock) {}

int main() {
	int sock = sock_connect("localhost", 444);
	if (sock == -1) {
		return 1;
	}

	return 0;
}
