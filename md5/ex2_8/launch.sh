#!/bin/bash
set -euo pipefail

make
./md.x param > out.dat
gnuplot plot.gnuplot