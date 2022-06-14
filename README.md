# SCC

Scotch containers. Container library used in the scotch language compiler.

## Dependencies

### Required

* A C++ compiler
* [cmake](https://cmake.org/)

### Optional

* [ccache](https://ccache.dev/)
* [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
* [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)
* [cmake-format](https://github.com/cheshirekow/cmake_format)
* [cppcheck](http://cppcheck.sourceforge.net/)
* [dot](https://graphviz.org/)
* [doxygen](https://www.doxygen.nl/index.html)

## Build tests

**Fetch and bootstrap vcpkg**

``` sh
git clone "https://github.com/Microsoft/vcpkg.git"
./vcpkg/bootstrap-vcpkg.sh -disableMetrics
./vcpkg/vcpkg install
```

* Or on Windows

``` posh
git clone "https://github.com/Microsoft/vcpkg.git"
.\vcpkg\bootstrap-vcpkg.bat -disableMetrics
.\vcpkg\vcpkg install
```

**Setup cmake**

``` sh
cmake -B build
```

**Build and run**

``` sh
cmake --build build
./build/bin/scc
```
