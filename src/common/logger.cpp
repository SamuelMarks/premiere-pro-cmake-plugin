/* Technically in `STANDALONE_MODE` this is a C file */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <xcselect.h>
#include <wchar.h>
#include <CoreFoundation/CoreFoundation.h>
#endif

#include "logger.h"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

const prUTF16Char* PROJECT_NAME_W =
#if defined(__APPLE__) && defined(__MACH__)
        (const prUTF16Char*)PROJECT_NAME
#elif defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    WIDEN(PROJECT_NAME)
#endif
        ;

#ifdef PLUGIN_MODE

static PrSDKErrorSuite3				*sErrorSuitePtr = NULL;

#endif


void set_error_suite_ptr(PrSDKErrorSuite3 *error_suite_ptr) {
#ifdef PLUGIN_MODE
    sErrorSuitePtr = error_suite_ptr;
#endif
}

void cleanup_error_suite_ptr() {
#ifdef PLUGIN_MODE
    // The caller must also do `spBasic->ReleaseSuite(kPrSDKErrorSuite, kPrSDKErrorSuiteVersion3)`
    sErrorSuitePtr = NULL;
#endif
}

int log_info(const char *message) {
    if (strlen(message) == 0) return EXIT_FAILURE;

#ifdef PLUGIN_MODE
    if (sErrorSuitePtr == NULL) return EXIT_FAILURE;

    fputs(message, stderr);
    prUTF16Char *w_msg = to_wchar(message);
    sErrorSuitePtr->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeInformational,
                                          w_msg,
                                          PROJECT_NAME_W
    );
    // free(w_msg);

#else
    puts(message);
#endif
    return EXIT_SUCCESS;
}

int log_info_w(const prUTF16Char *message) {
    if (message[0] == 0) return EXIT_FAILURE;

#ifdef PLUGIN_MODE
    if (sErrorSuitePtr == NULL) return EXIT_FAILURE;

    sErrorSuitePtr->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeInformational,
                                          const_cast<prUTF16Char *>(message),
                                          PROJECT_NAME_W
    );
#else
    printf("%"UTF16_FMT"\n", *message);
#endif
    return EXIT_SUCCESS;
}

int log_warn(const char *message) {
    if (strlen(message) == 0) return EXIT_FAILURE;

#ifdef PLUGIN_MODE
    if (sErrorSuitePtr == NULL) return EXIT_FAILURE;

    prUTF16Char *w_msg = to_wchar(message);
    sErrorSuitePtr->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeWarning,
                                          w_msg,
                                          PROJECT_NAME_W
    );
    free(w_msg);
#else
    fputs(message, stderr);
#endif
    return EXIT_SUCCESS;
}

int log_warn_w(const prUTF16Char *message) {
    if (message[0] == 0) return EXIT_FAILURE;

#ifdef PLUGIN_MODE
    if (sErrorSuitePtr == NULL) return EXIT_FAILURE;

    sErrorSuitePtr->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeWarning,
                                          const_cast<prUTF16Char *>(message),
                                          PROJECT_NAME_W
    );
#else
    fprintf(stderr, "%"UTF16_FMT"\n", *message);
#endif
    return EXIT_SUCCESS;
}

int log_error(const char *message) {
    if (strlen(message) == 0) return EXIT_FAILURE;

#ifdef PLUGIN_MODE
    if (sErrorSuitePtr == NULL) return EXIT_FAILURE;

    prUTF16Char *w_msg = to_wchar(message);
    sErrorSuitePtr->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeError,
                                          w_msg,
                                          PROJECT_NAME_W
    );
    free(w_msg);
#else
    fputs(message, stderr);
#endif
    return EXIT_SUCCESS;
}

int log_error_w(const prUTF16Char *message) {
    if (message[0] == 0) return EXIT_FAILURE;

#ifdef PLUGIN_MODE
    if (sErrorSuitePtr == NULL) return EXIT_FAILURE;

    sErrorSuitePtr->SetEventStringUnicode(PrSDKErrorSuite3::kEventTypeError,
                                          const_cast<prUTF16Char *>(message),
                                          PROJECT_NAME_W
    );
#else
    fprintf(stderr, "%"UTF16_FMT"\n", *message);
#endif
    return EXIT_SUCCESS;
}

const prUTF16Char* to_wchar(const char* message) {
    if (!message) return NULL;

    size_t len = strlen(message);
    int bufsize = (len + 1) * 2;
    char* buf = (char *) malloc(bufsize);

    int little_endian = 1;
    little_endian = ((char*)&little_endian)[0];
    memset(buf, 0, bufsize);
    {
        size_t i;
        for (i = 0; i < len; i++)
            buf[i * 2 + little_endian ? 0 : 1] = message[i];
    }

    return (prUTF16Char*)buf;
}

/*
// TODO: Remove magic numbers

void copy2ConvertStringLiteralIntoUTF16(const wchar_t* inputString, prUTF16Char* destination)
{
#if (defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)) && defined(PLUGIN_MODE)
    size_t length = wcslen(inputString);
    wcscpy_s(destination, length + 1, inputString);
#define strdup _strdup
#elif defined(__APPLE__) && defined(__MACH__)
    size_t length = wcslen(inputString);
    CFRange	range = {0, 150};
    range.length = length;
    CFStringRef inputStringCFSR = CFStringCreateWithBytes(	kCFAllocatorDefault,
                                                              reinterpret_cast<const UInt8 *>(inputString),
                                                              length * sizeof(wchar_t),
                                                              kCFStringEncodingUTF32LE,
                                                              false);
    CFStringGetBytes(	inputStringCFSR,
                         range,
                         kCFStringEncodingUTF16,
                         0,
                         false,
                         reinterpret_cast<UInt8 *>(destination),
                         length * (sizeof (prUTF16Char)),
                         NULL);
    destination[length] = 0; // Set NULL-terminator, since CFString calls don't set it, and MediaCore hosts expect it
    CFRelease(inputStringCFSR);
#endif
}

const prUTF16Char * to_wchar(const char* message) {
    const size_t cSize = strlen(message);
    wchar_t *w_str = new wchar_t[cSize];
    size_t outSize;
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    mbstowcs_s(&outSize, w_str, cSize, message, cSize-1);
    return w_str;
#else
    mbstowcs(w_str, message, cSize);
#endif
#if defined(__APPLE__) && defined(__MACH__)
    prUTF16Char *ut16str = new prUTF16Char[cSize];
    copy2ConvertStringLiteralIntoUTF16(w_str, ut16str);
    return ut16str;
#else
    return w_str;
#endif
}
*/
