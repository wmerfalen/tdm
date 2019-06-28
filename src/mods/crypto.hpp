#ifndef __MENTOC_MODS_CRYPTO_HEADER__
#define __MENTOC_MODS_CRYPTO_HEADER__
#include "crypto.hpp"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <string_view>

namespace mods::crypto {
#ifdef MENTOC_NO_CONSTEXPR_CONF
	static std::string hardcoded_key = "1218749!-Z0*$7611218749!-Z0*$761";
	static std::string hardcoded_iv = "1218749!-Z0*$761";
#else
	constexpr std::string_view hardcoded_key = "1218749!-Z0*$7611218749!-Z0*$761";
	constexpr std::string_view hardcoded_iv = "1218749!-Z0*$761";
#endif
	std::string encrypt_buffer(std::string_view in_key,std::string_view in_iv,std::string_view buffer);
	std::string decrypt_buffer(std::string_view in_ciphertext,std::string_view in_key,std::string in_iv,std::string buffer);
};

#endif
