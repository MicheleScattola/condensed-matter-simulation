#!/bin/bash

echo "set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font 'Arial,28'
set border lw 1.5 front
set out 'fits.pdf'

set style line 1 linecolor rgb 'red' linewidth 2;
set style line 2 linecolor rgb 'gold' linewidth 2;

set ylabel 'q'
set yrange [0:1]
set xlabel 't[ps]'
set title 'Refolding at 10K: k_{ba}=4E-3 k_{ab}=2.7E-3'

f(x)=a*(1-exp(-x/b))+c
a=1.0
b=0.1
c=0.0
fit f(x) 'average_refolding.dat' u 1:2 via a,b,c
kba = a/b
kab = (1-a)/b

print kab
print kba

print 1/kba
print 1/kab

p 'average_refolding.dat' w l ls 1 t 'Refolding average', f(x) w l ls 2"| gnuplot