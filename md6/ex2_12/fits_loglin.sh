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
set title 'Refolding at 10K'

set logscale x

p 'average_refolding.dat' w l ls 1 t 'Refolding average'"| gnuplot