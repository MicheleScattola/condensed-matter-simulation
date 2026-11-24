#!/bin/bash

echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "cumulative.pdf"

set style line 1 linecolor rgb "red" linewidth 2
set style line 2 linecolor rgb "blue" linewidth 2 dt 2

set xlabel "Simulation gamma"
set ylabel "Cumulative trials"
set y2label "Acceptance rate: N_{steps}/N_{trials}"
set y2tics
set title "Stop at: T >= T_{equilibrium}"

set format y "%.0e"
set logscale y
set key top center

p "data.dat" u 1:2 axes x1y1 w points pointtype 7 pointsize 1.5 linecolor rgb "red" title "Trials", \
  "data.dat" u 1:3 axes x1y2 w points pointtype 7 pointsize 1.5 linecolor rgb "navy" title "Acceptance rate"

set out
set term pop' | gnuplot