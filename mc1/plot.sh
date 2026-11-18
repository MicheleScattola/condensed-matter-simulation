#!/bin/bash

echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "cumulative.pdf"

set style line 1 linecolor rgb "red" linewidth 2
set style line 2 linecolor rgb "blue" linewidth 2 dt 2

set xlabel "Cumulative trials"
set ylabel "Temperature [K]"
set title "Gamma vs trials"

p "ave0.1.dat" u 3:2 w l lw 2, "ave0.2.dat" u 3:2 w l lw 2, "ave0.3.dat" u 3:2 w l lw 2,"ave0.4.dat" u 3:2 w l lw 2, "ave0.5.dat" u 3:2 w l lw 2, "ave0.6.dat" u 3:2 w l lw 2,"ave0.7.dat" u 3:2 w l lw 2, "ave0.8.dat" u 3:2 w l lw 2, "ave0.9.dat" u 3:2 w l lw 2,"ave1.0.dat" u 3:2 w l lw 2


set out
set term pop' | gnuplot