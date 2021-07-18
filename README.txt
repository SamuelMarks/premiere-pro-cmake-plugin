premiere-pro-cmake-plugin

[build and test] [License]

Adobe provides a C++ API to build plugins for its suite, including
Premiere Pro.

Unfortunately they provide separate MSVC and XCode project files,
meaning additional build flags and dependencies (like third-party
libraries) need to be manually added to each.

CMake has become the default ‘standard’ for cross-platform builds;
popularly for C and C++.

This project aims to be a good starter for cross-platform Premiere Pro
plugin development, including those that have third-party dependencies.

vcpkg

vcpkg is an open-source cross-platform library package management system
from Microsoft; targeting macOS, Linux, and Windows.

It’s very popular, and has strong CMake integration. Henceforth, it is
chosen for this project as an example of third-party library
integration.

Install vcpkg, configure it with your system. Then run:

    [root]/vcpkg/vcpkg install openssl zlib

Premiere Pro SDK

Due to licensing issues, this cannot be included directly. You must
specify it in the build step with -DADOBE_PP_SDK. Download the SDK here;
then unzip: https://console.adobe.io/servicesandapis

Tested with:

-   Premiere Pro CC 13.0 Mac SDK [CC 2019 ; Release 1 - Oct 2018]
    -   unzip and set -DADOBE_PP_SDK to that dir

Build

    $ mkdir 'cmake-build-debug' && cd "$_"
    $ cmake -DCMAKE_TOOLCHAIN_FILE='[root]/vcpkg/scripts/buildsystems/vcpkg.cmake' \
            -DADOBE_PP_SDK='[path_to_pp_sdk]' \
            -DCMAKE_BUILD_TYPE='Debug' \
            ..
    $ cmake --build .

------------------------------------------------------------------------

License

Licensed under either of

-   Apache License, Version 2.0 (LICENSE-APACHE or
    https://www.apache.org/licenses/LICENSE-2.0)
-   MIT license (LICENSE-MIT or https://opensource.org/licenses/MIT)

at your option.

Contribution

Unless you explicitly state otherwise, any contribution intentionally
submitted for inclusion in the work by you, as defined in the Apache-2.0
license, shall be dual licensed as above, without any additional terms
or conditions.
