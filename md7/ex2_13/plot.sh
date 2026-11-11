#!/bin/bash

echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "temp.pdf"

set multiplot layout 1,2 margins 0.1, 0.9, 0.15, 0.9 spacing 0, 0

set style line 1 linecolor rgb "red" linewidth 2
set style line 2 linecolor rgb "blue" linewidth 2

set ylabel "h(t)"
set xlabel "t [ps]"
set ytics nomirror
set title "T=19K"
unset y2tics
unset y2label

p "h19.dat"  w l ls 1 t "T=19K"

unset ylabel
unset ytics
set y2tics nomirror
set y2label "h(t)"
set xlabel "t [ps]"
set title "T=21K"

p "h21.dat"  w l ls 1 t "T=21K"
unset multiplot
set out
set term pop' | gnuplot