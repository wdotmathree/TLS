#include "net.hpp"
#include "proto.hpp"

// For testing purposes only
extern "C" size_t _ZN4Part9serializeEPhiz(...);

int main() {
	uint8_t buf[1024];
	ssize_t len = _ZN4Part9serializeEPhiz(buf, 4, 2, 0x8899, 1, 0xdd, 3, 0xaabbcc, 4, 0xdeadbeef);

	for (int i = 0; i < len; i++) {
		printf("%02x ", buf[i]);
	}
	printf("\n");

	return 0;
}
