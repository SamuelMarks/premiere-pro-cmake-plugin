#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <versions.h>
#include "configure.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

/* https://docs.microsoft.com/en-us/cpp/c-runtime-library/path-field-limits */
#define PATH_SEP "\\"
#define MAX_FNAME _MAX_FNAME
#define strtok_r strtok_s

#else

#define PATH_SEP "/"
#define MAX_FNAME PATH_MAX

#endif

int main(int argc, char *argv[]) {
    {
        char basename[MAX_FNAME] = {0};
        {
            char *token, *rest = NULL;
            for (token = strtok_r(argv[0], PATH_SEP, &rest); token != NULL; token = strtok_r(NULL, PATH_SEP, &rest))
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
                    strcpy_s(basename, MAX_FNAME, token);
#else
            strcpy(basename, token);
#endif
        }

        printf("%s version: %4s\n"
               "\nget_hash(): %20d\n", basename,
               PROJECT_VERSION,
               get_hash());
    }

    {
        size_t i;
        for (i = 0; i < VERSIONS_N; i++)
            wprintf(L"%ls: %14ls\n", VERSIONS[i][0], VERSIONS[i][1]);
    }

    return EXIT_SUCCESS;
}
