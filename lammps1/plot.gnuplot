set term push
set term pdfcairo enhanced color rounded size 7,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "performance.pdf"

set xrange [0.5:6.5]
set title "Parallelization performance"
set key bottom right box opaque
set xlabel "CPU cores"
set ylabel "Speedup:  t_{serial}/t_{parallel}"
p "data.dat" u 1:2 w l lw 2 title "Baseline", "data_10.dat" u 1:2 with l lw 2 title "Baseline x10"

set out
set term pop
