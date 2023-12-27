#pragma once
#ifndef DEFINES_H
#define DEFINES_H

#include "ec.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

enum ContentType {
	change_cipher_spec = 20,
	alert = 21,
	handshake = 22,
	application_data = 23,
}; // 1 byte

enum HandshakeType {
	client_hello = 1,
	server_hello = 2,
	new_session_ticket = 4,
	end_of_early_data = 5,
	encrypted_extensions = 8,
	certificate = 11,
	certificate_request = 13,
	certificate_verify = 15,
	finished = 20,
	key_update = 24,
	message_hash = 254,
}; // 1 byte

enum ExtensionType {
	server_name = 0,
	max_fragment_length = 1,
	status_request = 5,
	supported_groups = 10,
	signature_algorithms = 13,
	use_srtp = 14,
	heartbeat = 15,
	application_layer_protocol_negotiation = 16,
	signed_certificate_timestamp = 18,
	client_certificate_type = 19,
	server_certificate_type = 20,
	padding = 21,
	pre_shared_key = 41,
	early_data = 42,
	supported_versions = 43,
	cookie = 44,
	psk_key_exchange_modes = 45,
	certificate_authorities = 47,
	oid_filters = 48,
	post_handshake_auth = 49,
	signature_algorithms_cert = 50,
	key_share = 51,
}; // 2 bytes

struct ClientHello {
	uint16_t version; // 0x0303
	uint8_t random[32];
	uint8_t session_id_length; // 0
	uint16_t cipher_suites_length; // 2
	uint16_t cipher_suites[1]; // {0x13, 0x03}
	uint8_t compression_methods_length; // 1
	uint8_t compression_methods[1]; // {0}
	uint16_t extensions_length; // 5
	uint8_t extensions[7]; // {0, 43, 2, 0x03, 0x04}
} __attribute__((packed));

#endif
