# MSYS2 UCRT64 toolchain
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER /c/msys64/ucrt64/bin/gcc.exe)
set(CMAKE_CXX_COMPILER /c/msys64/ucrt64/bin/g++.exe)
set(CMAKE_C_FLAGS "-pipe")
set(CMAKE_CXX_FLAGS "-pipe")

# Find MSYS2 system packages
set(CMAKE_PREFIX_PATH "/c/msys64/ucrt64")
list(APPEND CMAKE_PREFIX_PATH "/c/msys64/ucrt64/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "/c/msys64/ucrt64/share/cmake")

# Tell find_package where to look
set(CMAKE_FIND_ROOT_PATH "/c/msys64/ucrt64")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
