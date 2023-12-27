#include "net.h"

int sock_send(int socket, const char *data, const size_t len, const uint8_t type) {
	char buf[16389];
	buf[0] = type;
	buf[1] = 3;
	buf[2] = 3;
	buf[3] = (len >> 8) & 0xff;
	buf[4] = len & 0xff;
	memcpy(buf + 5, data, len);
	return send(socket, buf, len + 5, 0);
}

int sock_recv(int socket, char *data, size_t *len) {
	char buf[16389];
	*len = recv(socket, buf, 16389, 0);
	if (*len < 0)
		return -1;
	if (buf[1] != 3 || buf[2] != 3)
		return -1;
	int size = (buf[3] << 8) | buf[4];
	*len -= 5;
	if (size != len)
		return -1;
	memcpy(data, buf + 5, size);
	return size;
}

int handshake_send(int socket, const char *data, const size_t len, const uint8_t type) {
	char buf[16389];
	buf[0] = type;
	buf[1] = len >> 16;
	buf[2] = (len >> 8) & 0xff;
	buf[3] = len & 0xff;
	memcpy(buf + 4, data, len);
	return sock_send(socket, buf, len + 4, handshake);
}

int handshake_recv(int socket, char *data, size_t *len) {
	char buf[16389];
	int tmp = sock_recv(socket, buf, len);
	if (tmp < 0)
		return -1;
	if (tmp != handshake)
		return -1;
	tmp = buf[0];
	int size = (buf[1] << 16) | (buf[2] << 8) | buf[3];
	*len -= 4;
	if (size != *len)
		return -1;
	memcpy(data, buf + 4, size);
	return tmp;
}
