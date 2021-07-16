#include "logger.h"
#include <string.h>

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

const wchar_t *VERSIONS[][2] = {
    {L"ZLIB_VERSION",         WIDEN(ZLIB_VERSION)},
    {L"OPENSSL_VERSION_TEXT", WIDEN(OPENSSL_VERSION_TEXT)}
};

const size_t VERSIONS_N = sizeof VERSIONS / sizeof VERSIONS[0];

uLong get_hash() {
#define BUF_LEN 5
    uLong crc = crc32(0L, Z_NULL, 0U);
    static Bytef buffer[BUF_LEN];
    memset(buffer, 0, BUF_LEN);
    return crc32(crc, buffer, BUF_LEN);
#undef BUF_LEN
}
