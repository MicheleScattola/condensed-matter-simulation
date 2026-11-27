set term pdfcairo enhanced color rounded size 6,5 font "Arial,16"
set output "thermalization.pdf"
set border lw 1.5

a = 120.0
b = 30.0
c = 20.0
f(x) = a * exp(-x / b) + c
fit [0:200] f(x) "out.dat" using 1:6 via a, b, c

set xlabel "Time [ps]"
set ylabel "Temperature [K]"
set title "Thermalization"
set key top right box opaque
set style line 1 linecolor rgb "web-green" lw 2
set style line 2 linecolor rgb "orange" lw 2
set style line 3 linecolor rgb "red" lw 2

plot [0:200] "out.dat" using 1:6 with lines ls 1 title "T_m", \
     "out.dat" using 1:7 with lines ls 2 title "T_w", \
     f(x) with lines ls 3 title sprintf("Fit: exp(-t/%.3f) K", b)

set output