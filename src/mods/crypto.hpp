#ifndef __MENTOC_MODS_CRYPTO_HEADER__
#define  __MENTOC_MODS_CRYPTO_HEADER__

#include <iostream>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#include "../globals.hpp"
#include "extern.hpp"
#include <libscrypt.h>

namespace mods {
	class crypto {
		public:
			constexpr static const char* salt = "tmnt1`9*$$jzz8N++";
			static int encrypt(const std::string & data,std::vector<char>& outbuf){
/**
 * crypto_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen):
 * Compute scrypt(passwd[0 .. passwdlen - 1], salt[0 .. saltlen - 1], N, r,
 * p, buflen) and write the result into buf.  The parameters r, p, and buflen
 * must satisfy r * p < 2^30 and buflen <= (2^32 - 1) * 32.  The parameter N
 * must be a power of 2 greater than 1.
 *
 * libscrypt_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen):
 * password; duh
 * N: CPU AND RAM cost (first modifier)
 * r: RAM Cost
 * p: CPU cost (parallelisation)
 * In short, N is your main performance modifier. Values of r = 8, p = 1 are
 * standard unless you want to modify the CPU/RAM ratio.
 * Return 0 on success; or -1 on error.
//int libscrypt_scrypt(const uint8_t *, size_t, const uint8_t *, size_t, uint64_t,
//    uint32_t, uint32_t, uint8_t *, size_t);
 */

				/* TODO: this is a guess at how much space we need */
				outbuf.reserve(data.length() * 2);
				return libscrypt_scrypt(reinterpret_cast<const uint8_t*>(data.c_str()),
						data.length(),
						reinterpret_cast<const uint8_t*>(salt),
						std::string(salt).length(),
						16,/* N - must be power of 2 greater than 1 */
						16,
						4,
						reinterpret_cast<uint8_t*>(&outbuf[0]),
						outbuf.size());
			}
	};
};

#endif
