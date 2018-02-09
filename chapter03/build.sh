#!/bin/bash

set -e

cd `dirname $0`/

make clean
make all

echo 'create TGA file'
./main tgaimage.o
# open output.tga
# open render.tga
# open scene.tga