#include <rexo.h>
#include "versions.h"

RX_TEST_CASE(versions, size) {
    RX_UINT_REQUIRE_EQUAL(VERSIONS_N, 2);
}

int main(int argc, const char **argv) {
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}
