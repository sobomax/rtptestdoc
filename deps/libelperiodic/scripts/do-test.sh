#!/bin/sh

set -e

CC=${CC:-cc}

uname -a
${CC} --version

./configure
make all
./configure --enable-coverage
make clean all
cd src
sh -x ./findskew.sh
