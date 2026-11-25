echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "multiplot.pdf"

set style line 2 linecolor rgb "red" linewidth 2
set style line 1 linecolor rgb "navy" linewidth 2
set style line 3 linecolor rgb "blue" linewidth 2 dt 2
set style line 4 linecolor rgb "orange" linewidth 2 dt 2

set key top right

set multiplot layout 1,2 margins 0.15, 0.95, 0.18, 0.9 spacing 0, 0


set ytics nomirror
set xlabel "time t [ns]"
set ylabel "gyration radius R_{g} [A]"
set title "MD convergence"
set parametric
set trange [5:35]
p "md_full.dat" u (($1)/1000):7 w l ls 1 title "MD", \
  0.1, t w l ls 3 title "transient"

set title "MC convergence"

unset ylabel
unset ytics
set xlabel "N_{steps}"
set format x "%.0tx10^{%T}"
p "mc_full.dat" u 1:8 w l ls 2 t "MC", \
    0.5e5, t w l ls 4 title "transient"

unset multiplot

set out
set term pop' | gnuplot