#!/bin/bash

export LC_NUMERIC=C

rm -f *.dat
make
./md.x parameters.8 | awk 'NR>10' > out.dat
awk -f ./2Dbinning.awk out.dat > prob.dat

awk 'BEGIN{k=0.08617718; t=300} NF >= 3 {print $1,$2,-k*t*log($3); next} {print ""}' prob.dat > free.dat

gnuplot plot.gnuplot