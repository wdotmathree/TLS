#include "defines.h"
#include "net.h"

uint16_t curr_suite = 0;
char *curr_key = NULL;
uint16_t next_suite;
char *next_key;

static const uint16_t cipher_suites[] = {
	0x1303, // TLS_CHACHA20_POLY1305_SHA256
};

static const uint16_t groups[] = {
	0x0017, // secp256r1
	0x0018, // secp384r1
	0x0019, // secp521r1
	0x0104, // ffdhe8192
};

static const uint16_t sig[] = {
	0x0403, // ecdsa_secp256r1_sha256
};

int rand_bytes(char *buf, size_t len) {
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return -1;
	int ret = read(fd, buf, len);
	close(fd);
	return ret;
}

int gen_key(uint16_t group, char *buf, char **secret) {
	int size = -1;
	if (group == 0x0017) {
		// secp256r1
		EC_init_curve("nistp256");
		size = 32;
	} else if (group == 0x0018) {
		// secp384r1
		EC_init_curve("nistp384");
		size = 48;
	} else if (group == 0x0019) {
		// secp521r1
		EC_init_curve("nistp521");
		size = 66;
	} else if (group == 0x0104) {
		// ffdhe8192
		size = 1024;
	} else {
		fprintf(stderr, "Unknown group: %04x\n", group);
		return -1;
	}
	*secret = malloc(size);
	if (*secret == NULL)
		return -1;
	if (rand_bytes(*secret, size) < 0)
		return -1;
	if (group != 0x0104) {
		EC_point g;
		EC_init_generator(&g);
		mpz_t n, sec;
		mpz_inits(n, sec, NULL);
		EC_order(n);
		mpz_import(sec, size, 1, 1, 0, 0, *secret);
		EC_mul(&g, &g, sec);
		mpz_clears(n, sec, NULL);
		int tmp = EC_serialize_point(&g, buf);
		EC_clear(&g);
		return tmp;
	}
	return -1;
}

int do_handshake(int socket, char *buf) {
	(void)socket;
	size_t pos = 0;
	*(uint16_t *)(buf + pos) = htons(0x0303);
	pos += 2;
	// random
	pos += rand_bytes(buf + pos, 32);
	buf[pos++] = 0; // session_id_length
	*(uint16_t *)(buf + pos) = htons(sizeof(cipher_suites)); // cipher_suites_length
	pos += 2;
	for (size_t i = 0; i < sizeof(cipher_suites) / sizeof(uint16_t); i++) {
		*(uint16_t *)(buf + pos) = htons(cipher_suites[i]);
		pos += 2;
	}
	buf[pos++] = 1; // compression_methods_length
	buf[pos++] = 0; // null
	pos += 2; // reserve extensions_length
	size_t tmp = pos;
	// supported_groups
	*(uint16_t *)(buf + pos) = htons(supported_groups);
	pos += 2;
	*(uint16_t *)(buf + pos) = htons(sizeof(groups) + 2);
	pos += 2;
	*(uint16_t *)(buf + pos) = htons(sizeof(groups));
	pos += 2;
	for (size_t i = 0; i < sizeof(groups) / sizeof(uint16_t); i++) {
		*(uint16_t *)(buf + pos) = htons(groups[i]);
		pos += 2;
	}
	// signature_algorithms
	*(uint16_t *)(buf + pos) = htons(signature_algorithms);
	pos += 2;
	*(uint16_t *)(buf + pos) = htons(sizeof(sig) + 2);
	pos += 2;
	*(uint16_t *)(buf + pos) = htons(sizeof(sig));
	pos += 2;
	for (size_t i = 0; i < sizeof(sig) / sizeof(uint16_t); i++) {
		*(uint16_t *)(buf + pos) = htons(sig[i]);
		pos += 2;
	}
	// supported_versions
	*(uint16_t *)(buf + pos) = htons(supported_versions);
	pos += 2;
	*(uint16_t *)(buf + pos) = htons(3);
	pos += 2;
	buf[pos++] = 2;
	*(uint16_t *)(buf + pos) = htons(0x0304);
	pos += 2;
	// key_share
	uint16_t CHOSEN_GROUP = 0x0017;
	*(uint16_t *)(buf + pos) = htons(key_share);
	pos += 10; // allocate 4 bytes for length, 2 bytes for group, and 2 more bytes for length
	*(uint16_t *)(buf + pos - 4) = htons(CHOSEN_GROUP);
	char *secret;
	int tmp2 = gen_key(CHOSEN_GROUP, buf + pos, &secret);
	if (tmp2 < 0)
		return -1;
	*(uint16_t *)(buf + pos - 8) = htons(tmp2 + 6);
	*(uint16_t *)(buf + pos - 6) = htons(tmp2 + 4);
	*(uint16_t *)(buf + pos - 2) = htons(tmp2);
	pos += tmp2;
	// populate length field
	*(uint16_t *)(buf + tmp - 2) = htons(pos - tmp);
	handshake_send(socket, buf, pos, client_hello);
	size_t len;
	handshake_recv(socket, buf, &len);
	return 0;
}

int main() {
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		perror("socket");
		return 1;
	}
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(443);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		perror("connect");
		return 1;
	}
	char buf[16384];
	do_handshake(s, buf);
}
