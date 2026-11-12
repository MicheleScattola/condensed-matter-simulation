#!/bin/bash

export LC_NUMERIC=C

#rm -f *.dat
make
#./md.x parameters.8 | awk 'NR>10' > out.dat
source ./2Dbinning.awk out.dat > prob.dat

awk 'BEGIN{k=0.08617718; t=300} {print $1,$2,-k*t*log(($3))}' prob.dat > free.dat

source ./plot.sh