git clone https://github.com/c0rp3n/fastnoise-simd.git
mkdir fastnoise-simd/build
cd fastnoise-simd/build
cmake -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX="../../deps/FastNoiseSIMD" ..
cmake --build . --target install
