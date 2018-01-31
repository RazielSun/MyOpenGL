#!/bin/bash

set -e
cd `dirname $0`/

g++ -ggdb -g3 -pg -O0 main.cpp tgaimage.cpp -o main
