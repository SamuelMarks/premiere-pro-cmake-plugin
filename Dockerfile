FROM debian:stable-slim as build

LABEL description="Build container"

RUN apt-get update -qq && apt-get install -y  \
    autoconf \
    binutils \
    build-essential \
    curl \
    gcc \
    g++ \
    git \
    libstdc++6 \
    libtool \
    make \
    ninja-build \
    pkg-config \
    tar \
    unzip \
    zip \
    && curl 'https://github.com/Kitware/CMake/releases/download/v3.21.2/cmake-3.21.2-linux-x86_64.sh' -Lo '/tmp/cmake.sh' \
    && chmod +x '/tmp/cmake.sh' && '/tmp/cmake.sh' --prefix='/usr/local' --skip-license \
    && mkdir '/vcpkg' \
    && git clone --depth=1 'https://github.com/Microsoft/vcpkg.git' '/vcpkg' \
    && '/vcpkg/bootstrap-vcpkg.sh' -useSystemBinaries \
    && VCPKG_FORCE_SYSTEM_BINARIES=1 '/vcpkg/vcpkg' install \
    inih \
    tinyfiledialogs \
    parson \
    civetweb \
    curl \
    grpc \
    abseil \
    protobuf \
    crc32c \
    nlohmann-json

RUN VCPKG_FORCE_SYSTEM_BINARIES=1 '/vcpkg/vcpkg' install \
    google-cloud-cpp

COPY ./ /pp

WORKDIR /pp/build

ADD 'Premiere_Pro_CC_13_Mac_SDK.zip' /pp_sdk

RUN cmake .. \
           -DCMAKE_BUILD_TYPE='Debug' \
           -DCMAKE_TOOLCHAIN_FILE='/vcpkg/scripts/buildsystems/vcpkg.cmake' \
           -DCRYPTO_LIB='OpenSSL' \
           -DADOBE_PP_SDK='/pp_sdk/Premiere Pro CC 13.0 Mac SDK' \
    && cmake . --build

FROM alpine:latest as runtime

LABEL description="Run container"

RUN apk update && apk add --no-cache \
    libstdc++

COPY --from=build '/src/build/pp' '/usr/local/pp/pp'

WORKDIR /usr/local/pp

CMD ./pp

EXPOSE 8080
