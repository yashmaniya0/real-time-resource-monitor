#!/bin/bash

python3 ./src/a.py

rmdir ./build

make build
./build/monitor
