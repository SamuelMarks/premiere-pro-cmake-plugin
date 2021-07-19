#ifndef PP_VERSIONS_H
#define PP_VERSIONS_H

#include <stddef.h>
#include <zlib.h>
#include <openssl/crypto.h>
#include "versions_export.h"

#ifdef	__cplusplus
extern "C" {
#endif

extern VERSIONS_EXPORT const wchar_t *VERSIONS[][2];
extern VERSIONS_EXPORT const size_t VERSIONS_N;

uLong VERSIONS_EXPORT get_hash();

#ifdef __cplusplus
}
#endif

#endif /* PP_VERSIONS_H */
