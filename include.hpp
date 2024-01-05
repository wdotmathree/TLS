#pragma once
#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>

typedef uint16_t CipherSuite;

// 1 byte
enum ContentType {
	change_cipher_spec = 20,
	alert = 21,
	handshake = 22,
	application_data = 23,
} __attribute__((packed));

// 1 byte
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
} __attribute__((packed));

// 2 bytes
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
	_ = 0xffff,
} __attribute__((packed));

// 2 bytes
enum CipherSuite {
	TLS_AES_128_GCM_SHA256 = 0x1301,
	TLS_AES_256_GCM_SHA384 = 0x1302,
	TLS_CHACHA20_POLY1305_SHA256 = 0x1303,
	TLS_AES_128_CCM_SHA256 = 0x1304,
	TLS_AES_128_CCM_8_SHA256 = 0x1305,
} __attribute__((packed));

typedef struct Handshake {
	HandshakeType msg_type;
	uint32_t length;
	uint8_t *body;
} __attribute__((packed)) Handshake;

typedef struct Extension {
	ExtensionType type;
	uint16_t length;
	uint8_t *data;
} __attribute__((packed)) Extension;

struct ClientHello {
	uint16_t legacy_version; // MUST be 0x0303 indicating TLS 1.2
	uint8_t random[32]; // 32 bytes of random data
	uint8_t legacy_session_id_length; // Must be 0 (the session id vector has been omitted here)
	uint16_t cipher_suites_length;
	CipherSuite *cipher_suites;
	uint8_t legacy_compression_methods_length; // MUST be 1
	uint8_t legacy_compression_methods[1]; // MUST be 0
	uint16_t extensions_length;
	Extension *extensions;
} __attribute__((packed)) __attribute__((scalar_storage_order("big")));

struct ServerHello {
	uint16_t legacy_version; // MUST be 0x0303 indicating TLS 1.2
	uint8_t random[32]; // 32 bytes of random data
	uint8_t legacy_session_id_echo_length; // Must be 0, otherwise we abort
	uint16_t cipher_suites_length;
	CipherSuite *cipher_suites;
	uint8_t legacy_compression_method; // MUST be 0
	uint16_t extensions_length;
	Extension *extensions;
} __attribute__((packed)) __attribute__((scalar_storage_order("big")));

#endif
