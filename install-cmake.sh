DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
mkdir -p ${DEPS_DIR} && cd ${DEPS_DIR}
CMAKE_URL="https://cmake.org/files/v3.16/cmake-3.16.2-Linux-x86_64.tar.gz"
mkdir cmake && travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
export PATH=${DEPS_DIR}/cmake/bin:${PATH}
