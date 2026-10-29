#!/bin/bash

seeds=(4567 4597 4637 4657 4787 4817 4877 4937 4957 4967)

./md.x param ${seeds[0]} | awk '{print $1,$6}' > tmp.dat

for i in {1..9}; do
    echo "./md.x param ${seeds[$i]} | awk '{print $1,$6}' > out_$((i+1)).dat"
    ./md.x param ${seeds[$i]} | awk '{print $1,$6}' > out_$((i+1)).dat
    paste tmp.dat out_$((i+1)).dat > tmp2.dat
    awk '{print $1,$2+$4}' tmp2.dat > tmp.dat
done

awk '{print $1,$2/10}' tmp.dat > average.dat

echo "set title '10 seed average - Temperature';
set xlabel 't [ps]';
set ylabel 'T [K]';
set key bottom right;
p 'out_4.dat' u 1:2 w l lw 1 t 'data 4', 'out_7.dat' u 1:2 w l lw 1 t 'data 7', 'out_2.dat' u 1:2 w l lw 1 t 'data 2', 'average.dat' u 1:2 w l lw 3 t 'Average';
@pdfplot" | gnuplot

for i in {2..10}; do
    rm out_$i.dat
done

rm tmp.dat tmp2.dat