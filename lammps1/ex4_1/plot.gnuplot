set term push
set term pdfcairo enhanced color rounded size 7,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "performance.pdf"

set style line 1 linecolor rgb "red" lw 2
set style line 2 linecolor rgb "orange" lw 2

set xrange [0.5:6.5]
set title "Fixed density performance"
set key bottom right box opaque
set xlabel "CPU cores"
set ylabel "Speedup"
p "data.dat" u 1:2 w l ls 1 title "10^{2} atoms", "data_10.dat" u 1:2 with l ls 2 title "10^{3} atoms"

set out
set term pop
