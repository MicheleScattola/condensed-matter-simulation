set term pdfcairo enhanced color rounded size 7,5 font "Arial,16"
set output "thermalization.pdf"
set border lw 1.5

Tset = 100.0
A1 = -70.0
tau1 = 5.0
A2 = -80.0
tau2 = 50.0
f1(x) = Tset + A1 * exp(-x / tau1)
f2(x) = Tset + A2 * exp(-x / tau2)

fit [0:200] f1(x) "out1.dat" using 1:6 via A1, tau1
fit [0:200] f2(x) "out2.dat" using 1:6 via A2, tau2

set xlabel "Time [ps]"
set ylabel "Temperature [K]"
set title "Langevin thermalization"
set key bottom right box opaque
set style line 1 linecolor rgb "web-green" lw 2
set style line 2 linecolor rgb "orange" lw 2
set style line 3 linecolor rgb "dark-green" lw 3 
set style line 4 linecolor rgb "red" lw 3 
set style line 5 linecolor rgb "gray50" lw 2

plot [0:200] "out1.dat" using 1:6 with lines ls 1 title "T_m, {/Symbol g}=1.0", \
     "out2.dat" using 1:6 with lines ls 2 title "T_m, {/Symbol g}=0.1", \
     Tset with lines ls 5 title "Thermostat: 100 K", \
     f1(x) with lines ls 3 title sprintf("Fit {/Symbol g}=1.0: {/Symbol t}=%.0f ps", tau1), \
     f2(x) with lines ls 4 title sprintf("Fit {/Symbol g}=0.1: {/Symbol t}=%.0f ps", tau2)

set output