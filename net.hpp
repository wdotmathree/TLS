#pragma once
#ifndef NET_HPP
#define NET_HPP

#include "include.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>

class Socket {
private:
	int fd = -1;

public:
	Socket();
	Socket(const char *host, const int port);
	~Socket();

	void connect(const char *host, const int port);
	void close();
	void set_blocking(const bool blocking);
	ssize_t send(const void *buf, const size_t len);
	ssize_t recv(void *buf, const size_t len);
};

#endif
