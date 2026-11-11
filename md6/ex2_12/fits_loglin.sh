#!/bin/bash

echo "set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font 'Arial,28'
set border lw 1.5 front
set out 'loglin.pdf'

set style line 1 linecolor rgb 'red' linewidth 3;
set style line 2 linecolor rgb 'royalblue' linewidth 2;
set style line 3 linecolor rgb 'violet' linewidth 2;

set ylabel 'q'
set yrange [0:1]
set xlabel 't [ps]'
set title 'Double exp sum: tau_{1}=20.7 ps tau_{2}=186 ps'
set logscale x

A1=0.2
tau1=10.0
A2=0.6
tau2=200.0
C=0.08

F(x) = A1*(1-exp(-x/tau1)) + A2*(1-exp(-x/tau2)) + C

fit [1:] F(x) 'average_refolding.dat' u 1:2 via A1, tau1, A2, tau2, C

plot 'average_refolding.dat' w l ls 1 t 'Refolding average', \
     [1:] F(x) w l ls 2 t 'Double Exponential Fit'
" | gnuplot