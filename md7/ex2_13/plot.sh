#!/bin/bash

echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "temp.pdf"

set style line 1 linecolor rgb "red" linewidth 2
set style line 2 linecolor rgb "blue" linewidth 2

set logscale y

set ylabel "h(t)"
set xlabel "t [ns]"
set ytics nomirror

set multiplot layout 1,2 margins 0.15, 0.95, 0.15, 0.9 spacing 0, 0

set title "T=19K"

p "h19.dat" u (($1+18)/1000):2  w l ls 1 t "T=19K"

unset ylabel
unset ytics
set xlabel "t [ns]"
set title "T=21K"

p "h21.dat" u (($1+18)/1000):2 w l ls 1 t "T=21K"

unset multiplot
set out
set term pop' | gnuplot