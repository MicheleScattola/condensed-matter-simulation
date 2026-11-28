set term push
set term pdfcairo enhanced color rounded size 8,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "cumulative.pdf"

set style line 1 linecolor rgb "red" linewidth 2 pt 7 ps 0.8
set style line 2 linecolor rgb "blue" linewidth 2 pt 7 ps 0.8

set xlabel "Simulation {/Symbol g}" textcolor rgb "red"
set ylabel "Cumulative trials"
set x2label "<Acceptance rate>" textcolor rgb "blue"

set xtics nomirror
set x2tics nomirror
p "data.dat" u 1:2 axes x1y1 w lp ls 1 notitle, \
  "data.dat" u 3:2 axes x2y1 w lp ls 2 notitle 
set out
set term pop