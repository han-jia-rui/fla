set shell := ["zsh", "-uc"]

default:build

build:
    cmake -S . -B build
    cmake --build build --parallel 4

test: build
    cmake --build build --target test

pytest: build
    pytest ./python

docs: build
    cmake --build build --target docs

clean:
    rm -rf ./build ./bin

line-count:
    cloc ./fla-project ./python
