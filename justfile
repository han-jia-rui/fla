set shell := ["zsh", "-uc"]

default:build

build:
    cmake -S . -B build
    cmake --build build

test: build
    cmake --build build --target test

docs: build
    cmake --build build --target docs

clean:
    rm -rf ./build ./bin
