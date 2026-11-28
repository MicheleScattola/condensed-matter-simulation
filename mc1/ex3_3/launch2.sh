#!/bin/bash

export LC_NUMERIC=C

rm -f *.dat
make

./md.x mdparams > md_full.dat
./mc.x mcparams > mc_full.dat

awk -v col=7 -v tmin=100 -v tmax=1e7 -f ./1Dbinning.awk md_full.dat > mdout.dat
awk -v col=8 -v tmin=1e5 -v tmax=1e7 -f ./1Dbinning.awk mc_full.dat > mcout.dat

gnuplot plot.gnuplot