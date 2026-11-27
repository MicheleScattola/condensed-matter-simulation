#!/bin/bash
set -euo pipefail

rm -r *.dat
rm fit.log
make

./md.x param1 > out1.dat
./md.x param2 > out2.dat

gnuplot plot.gnuplot
