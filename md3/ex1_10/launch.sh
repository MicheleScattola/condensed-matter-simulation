#!/bin/bash
set -euo pipefail

rm -r *.dat
rm fit.log
make

./md.x param1 > out1.dat
./md.x param1.1 > out1.1.dat
./md.x param2 > out2.dat
./md.x param2.1 > out2.1.dat

gnuplot plot.gnuplot
