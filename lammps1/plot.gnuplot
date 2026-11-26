set term push
set term pdfcairo enhanced color rounded size 7,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "performance.pdf"

set xrange [0.5:6.5]
set title "Fixed density performance"
set key bottom right box opaque
set xlabel "CPU cores"
set ylabel "Speedup"
p "data.dat" u 1:2 w l lw 2 title "10^{2} atoms", "data_10.dat" u 1:2 with l lw 2 title "10^{3} atoms"

set out
set term pop
