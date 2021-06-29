#ifndef PP_VERSIONS_H
#define PP_VERSIONS_H

// #include <string>

#include "../common/logger.h"

#include <zlib.h>
#include <openssl/crypto.h>

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

static const wchar_t *VERSIONS[][2] = {
        {L"ZLIB_VERSION", WIDEN(ZLIB_VERSION)},
        {L"OPENSSL_VERSION_TEXT", WIDEN(OPENSSL_VERSION_TEXT)}
};

#endif /* PP_VERSIONS_H */
