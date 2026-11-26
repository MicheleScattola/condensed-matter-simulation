set term push
set term pdfcairo enhanced color rounded size 7,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out outname

set xrange [0.5:6.5]
set title titlename
set key bottom right box opaque
set xlabel "CPU cores"
set ylabel "Speedup:  t_{serial}/t_{parallel}"
p filename u 1:2 with points pt 7 title "mpi runs"

set out
set term pop