#include "net.hpp"

Socket::Socket() {
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd == -1) {
		throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
	}
}

Socket::Socket(const char *host, const int port) {
	this->connect(host, port);
}

Socket::~Socket() {
	this->close();
}

void Socket::connect(const char *host, const int port) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if (getaddrinfo(host, std::to_string(port).c_str(), &hints, &result) < 0) {
		throw std::runtime_error("Failed to get address info: " + std::string(strerror(errno)));
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		if (::connect(this->fd, rp->ai_addr, rp->ai_addrlen) != -1) {
			break; // Success
		}
	}

	if (rp == NULL) {
		throw std::runtime_error("Failed to connect to " + std::string(host) + ":" + std::to_string(port));
	}

	freeaddrinfo(result);
}

void Socket::close() {
	if (this->fd != -1) {
		::close(this->fd);
		this->fd = -1;
	}
}

void Socket::set_blocking(const bool blocking) {
	int flags = fcntl(this->fd, F_GETFL, 0);
	if (flags == -1) {
		throw std::runtime_error("Failed to get socket flags: " + std::string(strerror(errno)));
	}

	if (blocking) {
		flags &= ~O_NONBLOCK;
	} else {
		flags |= O_NONBLOCK;
	}

	if (fcntl(this->fd, F_SETFL, flags) == -1) {
		throw std::runtime_error("Failed to set socket flags: " + std::string(strerror(errno)));
	}
}

ssize_t Socket::send(const void *buf, const size_t len) {
	ssize_t ret = ::send(this->fd, buf, len, 0);
	if (ret == -1) {
		throw std::runtime_error("Failed to send data: " + std::string(strerror(errno)));
	}
	return ret;
}

ssize_t Socket::recv(void *buf, const size_t len) {
	ssize_t ret = ::recv(this->fd, buf, len, 0);
	if (ret == -1) {
		throw std::runtime_error("Failed to receive data: " + std::string(strerror(errno)));
	}
	return ret;
}
