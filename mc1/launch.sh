#!/bin/bash

export LC_NUMERIC=C

rm -f *.dat
make
./mc.x parameters.1 0.1 > out0.1.dat

# progressive gamma
for i in {1..9}; do
    ./mc.x parameters.1 0.$i > out0.$i.dat
    awk '{t=t+$7;  print $1,$6,t}' out0.$i.dat > ave0.$i.dat
done    
./mc.x parameters.1 1.0 > out1.0.dat
awk '{t=t+$7;  print $1,$6,t}' out1.0.dat > ave1.0.dat

source plot.sh