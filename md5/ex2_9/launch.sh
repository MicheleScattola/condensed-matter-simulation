#!/bin/bash

seeds=(4567 4597 4637 4657 4787 4817 4877 4937 4957 4967)
rm *.dat
make clean
make
./md.x param ${seeds[0]} | awk '{print $1,$6}' > tmp.dat

for i in {1..9}; do
    echo "./md.x param ${seeds[$i]} | awk '{print $1,$6}' > out_$((i+1)).dat"
    ./md.x param ${seeds[$i]} | awk '{print $1,$6}' > out_$((i+1)).dat
    paste tmp.dat out_$((i+1)).dat > tmp2.dat
    awk '{print $1,$2+$4}' tmp2.dat > tmp.dat
done

awk '{print $1,$2/10}' tmp.dat > average.dat

gnuplot plot.gnuplot

for i in {2..10}; do
    rm out_$i.dat
done

rm tmp.dat tmp2.dat