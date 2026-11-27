set term pdfcairo enhanced color rounded size 5,5 font "Arial,19"
set output "temperature_decay.pdf"
set border lw 1.5

a = 70.0
b = 10.0
f(x) = a * exp(-x / b)
fit f(x) "out.dat" using 1:6 via a, b

set xlabel "Time [ps]"
set ylabel "Temperature [K]"
set title "Temperature decay"
set key top right box opaque
set style line 1 linecolor rgb "web-green" lw 2
set style line 2 linecolor rgb "red" lw 2

plot [0:50] "out.dat" using 1:6 with lines ls 1 title "T_m", \
     f(x) with lines ls 2 title sprintf("Fit: exp(-t/%.1f) K", b)

set output