#pragma once
#ifndef NET_H
#define NET_H

#include "defines.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>

/**
 * @brief Creates a TCP socket and connects to the given host and port
 *
 * @param host The host to connect to
 * @param port The port to connect to
 * @return The socket file descriptor or -1 on error
 */
int sock_connect(const char *host, const int port);

#endif
