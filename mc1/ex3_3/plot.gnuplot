set term push
set term pdfcairo enhanced color rounded size 6,6 lw 1.5 font "Arial,18"
set border lw 1.5 front
set key box opaque

set out "comparison.pdf"

set style line 1 linecolor rgb "red" lw 2
set style line 2 linecolor rgb "navy" lw 2

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
set ylabel "Free energy [meV]"
set yrange [0:70]
set xlabel "Gyration radius R_{g} [A]"
set format x "%g"
p "mcout.dat" u 1:3 w l ls 1 t "MC", "mdout.dat" u 1:3 w l ls 2 t "MD"

unset multiplot
unset xrange
unset yrange

# transient plot
set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "multiplot.pdf"

set style line 2 linecolor rgb "red" lw 2
set style line 1 linecolor rgb "navy" lw 2
set style line 3 linecolor rgb "blue" lw 3 dt 4
set style line 4 linecolor rgb "orange" lw 3 dt 4

set key top right

set multiplot layout 1,2 margins 0.15, 0.95, 0.18, 0.9 spacing 0, 0


set ytics nomirror
set xlabel "time t [ns]"
set ylabel "gyration radius R_{g} [A]"
set title "MD convergence"
set parametric
set trange [0:35]
set xrange [0:2]
p "md_full.dat" u (($1)/1000):7 w l ls 1 title "MD", \
  0.1, t w l ls 3 title "transient"

set title "MC convergence"

unset ylabel
unset ytics
unset xrange
set xlabel "N_{steps}"
set format x "%.0tx10^{%T}"
p "mc_full.dat" u 1:8 w l ls 2 t "MC", \
    1.0e5, t w l ls 4 title "transient"

unset multiplot
set out
set term pop