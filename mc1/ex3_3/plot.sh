#!/bin/bash

echo 'set term push
set term pdfcairo enhanced color rounded size 6,6 lw 1.5 font "Arial,18"
set border lw 1.5 front
set out "comparison.pdf"

set style line 1 linecolor rgb "red" linewidth 2
set style line 2 linecolor rgb "navy" linewidth 2

stats "mcout.dat" using 1 nooutput
mc_xmin = STATS_min
mc_xmax = STATS_max
stats "mdout.dat" using 1 nooutput
md_xmin = STATS_min
md_xmax = STATS_max
shared_xmin = (mc_xmin < md_xmin ? mc_xmin : md_xmin)
shared_xmax = (mc_xmax > md_xmax ? mc_xmax : md_xmax)
set xrange [shared_xmin:shared_xmax]

set key top right
set multiplot layout 2,1 margins 0.14, 0.96, 0.1, 0.94 spacing 0, 0

set title "MD vs MC simulation at 100K"
set ylabel "P(R_{g})"
set yrange [0:0.4]
unset xlabel
set format x ""
p "mcout.dat" u 1:2 w l ls 1 t "MC", "mdout.dat" u 1:2 w l ls 2 t "MD"

unset title
unset key
set ylabel "Free energy F"
set yrange [0:70]
set xlabel "Gyration radius R_{g} [A]"
set format x "%g"
p "mcout.dat" u 1:3 w l ls 1 t "MC", "mdout.dat" u 1:3 w l ls 2 t "MD"

unset multiplot
set out
set term pop' | gnuplot
