set shell := ["zsh", "-uc"]

default:build

build:
  cmake -S . -B build
  cmake --build build

test:
  CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

clean:
  rm -rf ./build
