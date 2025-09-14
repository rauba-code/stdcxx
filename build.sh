#!/bin/sh
echo "$(find src -type f -name '*.cpp' -printf "'%f', " | sed -e 's:, $:):; s:^:sources = files(:')" | tee src/meson.build &&
  meson setup build --reconfigure &&
  meson compile -C build &&
  ./build/stdcxx
