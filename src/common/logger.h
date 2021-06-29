#ifndef PP_LOGGER_H
#define PP_LOGGER_H

//#include <libcommon_and_static_export.h>
#include "../configure/configure.h"

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

#if (defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)) && defined(PLUGIN_MODE)
typedef	unsigned short int	prUTF16Char;
#else
typedef wchar_t		prUTF16Char;
#endif

#endif

extern prUTF16Char * to_wchar(const char*);
extern void set_error_suite_ptr(PrSDKErrorSuite3 *);
extern void cleanup_error_suite_ptr();

extern int log_info(const char *);
extern int log_info_w(const wchar_t *);

extern int log_warn(const char *);
extern int log_warn_w(const wchar_t *);

extern int log_error(const char *);
extern int log_error_w(const wchar_t *);

#ifdef    __cplusplus
}
#endif

#endif /* PP_LOGGER_H */
