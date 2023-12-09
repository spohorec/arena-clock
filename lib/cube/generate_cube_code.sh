#!/bin/bash

CUBE=~/tools/STM32CubeMX/STM32CubeMX

echo "Starting STM32Cube Code Gen..."
pushd lib/cube/

rm -rf Src/
rm -rf Inc/

java -jar $CUBE -q script -s

rm -rf MXTmpFiles

patch -p1 Src/main.c patches/main.c.patch

popd
