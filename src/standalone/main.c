#include <stdlib.h>

#include <logger.h>
#include <versions.h>

int main(void) {
    size_t i;
    for(i=0; i<VERSIONS_N; i++)
        log_info_w(VERSIONS[i][0]),
        log_info_w(VERSIONS[i][1]);
    return EXIT_SUCCESS;
}
