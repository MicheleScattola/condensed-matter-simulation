#!/bin/bash

echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "temp.pdf"

set style line 1 linecolor rgb "red" linewidth 2
set style line 2 linecolor rgb "blue" linewidth 2 dt 2

set logscale y

set ylabel "h(t)"
set xlabel "t [ns]"
set ytics nomirror

set multiplot layout 1,2 margins 0.15, 0.95, 0.15, 0.9 spacing 0, 0

set title "T=9K : tau_{fit} = 0.75"

a=1.0
b=0.7
f(x) = 1 + a*exp(-x/b)
fit [0.6:] f(x) "h9.dat" u (($1)/1000):2 via a,b
p "h9.dat" u (($1)/1000):2  w l ls 1 t "T=9K", [0.6:] f(x) w l ls 2 t "Fit" 

unset ylabel
unset ytics
set xlabel "t [ns]"
set title "T=11K : tau_{fit} = 0.15"

c=1.0
d=0.7
g(x) = 1 + c*exp(-x/d)
fit [0.6:] g(x) "h11.dat" u (($1)/1000):2 via c,d
p "h11.dat" u (($1)/1000):2  w l ls 1 t "T=11K", [0.6:] g(x) w l ls 2 t "Fit" 

unset multiplot
set out
set term pop' | gnuplot