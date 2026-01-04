root=$(pwd)
cd /mnt/LinuxData/clang_luabind_test/build/
cmake -S .. -B . \
    -G "Ninja" \
    -DCMAKE_C_COMPILER="/mnt/LinuxData/pragma_develop/deps/staging/clang/bin/clang" \
    -DCMAKE_CXX_COMPILER="/mnt/LinuxData/pragma_develop/deps/staging/clang/bin/clang++" \
    -DCMAKE_INSTALL_PREFIX:PATH="/mnt/LinuxData/clang_luabind_test/install" \
    -DCMAKE_CXX_STDLIB_MODULES_JSON="/usr/lib/libstdc++.modules.json" \
    -DLUA_INCLUDE_DIR="/mnt/LinuxData/pragma_develop/deps/staging/luajit/include/" \
    -DLUA_LIBRARY="/mnt/LinuxData/pragma_develop/deps/staging/luajit/lib/libluajit-p.so" \
    -DCMAKE_CXX_SCAN_FOR_MODULES=1 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
    -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install .
