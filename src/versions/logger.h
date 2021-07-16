#ifndef PP_LOGGER_H
#define PP_VERSIONS_H

#include <stddef.h>
#include <zlib.h>
#include <openssl/crypto.h>
#include "versions_export.h"

extern VERSIONS_EXPORT const wchar_t *VERSIONS[][2];
extern VERSIONS_EXPORT const size_t VERSIONS_N;
uLong VERSIONS_EXPORT get_hash();

#endif /* PP_LOGGER_H */
