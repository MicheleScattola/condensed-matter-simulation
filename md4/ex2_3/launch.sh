#!/bin/bash
set -euo pipefail

make

./md.x param > out1.5.dat
./md.x param > out2.0.dat
./md.x param > out3.0.dat

./md.x param 1.5 0 > 1.5.dat
./md.x param 1.5 1 > shift1.5.dat
./md.x param 2.0 0 > 2.0.dat
./md.x param 2.0 1 > shift2.0.dat
./md.x param 3.0 0 > 3.0.dat
./md.x param 3.0 1 > shift3.0.dat

gnuplot plot.gnuplot