#ifndef PP_LOGGER_H
#define PP_LOGGER_H

#include "common_export.h"
#include <configure.h>

#ifdef	__cplusplus
extern "C" {
#endif

#if (defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)) && defined(PLUGIN_MODE)
#define NOWINBASEINTERLOCK
#include <intrin.h>
#endif

#ifdef PLUGIN_MODE
#include <SPBasic.h>
#include <PrSDKErrorSuite.h>


// Function to convert and copy string literals to the format expected by the exporter API.
// On Win: Pass the input directly to the output
// On Mac: All conversion happens through the CFString format
extern void copy2ConvertStringLiteralIntoUTF16(const wchar_t*, prUTF16Char*);

#else
typedef void* PrSDKErrorSuite3;

#if defined(__APPLE__) && defined(__MACH__)
typedef unsigned short int prUTF16Char;
#else
typedef wchar_t	prUTF16Char;
#endif

extern const prUTF16Char* PROJECT_NAME_W;

#endif

extern COMMON_EXPORT const prUTF16Char * to_wchar(const char*);
extern COMMON_EXPORT void set_error_suite_ptr(PrSDKErrorSuite3 *);
extern COMMON_EXPORT void cleanup_error_suite_ptr();

extern COMMON_EXPORT int log_info(const char *);
extern COMMON_EXPORT int log_info_w(const prUTF16Char *);

extern COMMON_EXPORT int log_warn(const char *);
extern COMMON_EXPORT int log_warn_w(const prUTF16Char *);

extern COMMON_EXPORT int log_error(const char *);
extern COMMON_EXPORT int log_error_w(const prUTF16Char *);

#ifdef    __cplusplus
}
#endif

#endif /* PP_LOGGER_H */
