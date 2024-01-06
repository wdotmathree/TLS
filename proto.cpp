#include "proto.hpp"

size_t Part::serialize(uint8_t *out, int num_members, ...) {
	va_list args;
	va_start(args, num_members);

	size_t len = 0;
	while (num_members--) {
		int type = va_arg(args, int);
		if (type == -1) {
			// Part
			len += va_arg(args, Part *)->serialize(out + len);
		} else {
			// Some type of integer, byte swap and copy
			*(uint64_t *)(out + len) = htobe64(va_arg(args, uint64_t)) >> (8 * (8 - type));
			len += type;
		}
	}

	va_end(args);
	return len;
}
