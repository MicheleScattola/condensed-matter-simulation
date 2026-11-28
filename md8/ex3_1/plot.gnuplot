set term push
set term pdfcairo enhanced color rounded size 16,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "2Dplots.pdf"

set multiplot layout 1,2 margins 0.1, 0.9, 0.2, 0.87 spacing 0.2, 0
set view map
set pm3d map interpolate 4,4

stats "prob.dat" u 1
set xrange [STATS_min:STATS_max]
stats "prob.dat" u 2
set yrange [STATS_min:STATS_max]

set cblabel "p(q,R_{g})" rotate by 0 offset -5.5, 8.5
set xlabel "head-tail distance q [A]"
set ylabel "gyration radius R_{g} [A]"
splot "prob.dat" using 1:2:3 notitle w pm3d

set cblabel "F(q,R_{g}) [meV]"
set xlabel "head-tail distance q [A]"
set ylabel "gyration radius R_{g} [A]"
splot "free.dat" using 1:2:($3 < 1e300 ? $3 : 1/0) notitle w pm3d

unset multiplot

set out
set term pop