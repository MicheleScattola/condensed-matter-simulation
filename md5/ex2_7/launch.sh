#!/usr/bin/env bash
set -euo pipefail

make

./md.x param1 > out1.dat
./md.x param2 > out2.dat

gnuplot plot.gnuplot