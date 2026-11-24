echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "multiplot.pdf"

set style line 2 linecolor rgb "red" linewidth 2
set style line 1 linecolor rgb "navy" linewidth 2

set format y "%.0e"
set format y2 "%.0e"

set key top right

set multiplot layout 1,2 margins 0.15, 0.95, 0.18, 0.9 spacing 0, 0


set ytics nomirror
set xlabel "time t [ns]"
set ylabel "gyration radius R_{g} [A]"
set title "MD convergence"

p "md_full.dat" u (($1)/1000):7 w l ls 1 t "MD"

set title "MC convergence"

unset ylabel
unset ytics
set xlabel "N_{steps}"
set format x "%.0e"
p "mc_full.dat" u 1:8 w l ls 2 t "MC"

unset multiplot

set out
set term pop' | gnuplot