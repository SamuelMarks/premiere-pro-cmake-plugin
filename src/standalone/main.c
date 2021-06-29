#include <stdlib.h>

#include "../common/logger.h"
#include "../versions/versions.h"

int main(void) {
    for(unsigned i=0; i<sizeof VERSIONS/sizeof VERSIONS[0]; i++)
        log_info_w(VERSIONS[i][0]),
        log_info_w(VERSIONS[i][1]);
    return EXIT_SUCCESS;
}
