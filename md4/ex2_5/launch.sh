#!/bin/bash

set -euo pipefail

make
./md.x param 2.0 1 > out.dat
gnuplot plot.gnuplot