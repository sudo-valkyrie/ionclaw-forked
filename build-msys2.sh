#!/usr/bin/env bash
# Run this from the MSYS2 UCRT64 shell
set -euo pipefail

cd "$(dirname "$0")"
echo "=== Working dir: $(pwd) ==="
echo "=== Compiler: $(which gcc) ==="
echo "=== Make: $(which make) ==="

rm -rf build/release
cmake -B build/release -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cmake --build build/release -j$(nproc)
echo "=== Build exit code: $? ==="
