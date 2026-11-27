set term pdfcairo enhanced color rounded size 7,5 font "Arial,16"
set border lw 1.5
set key top left
set xlabel "Recursion count"
set ylabel "Mean recursion time [ns]"
set grid

a = 1.0
b = 1.0
c = 1.0
d = 1.0
fit a*x 'recurs1.dat' using 0:($1/1000) via a
fit b*x 'recurs2.dat' using 0:($1/1000) via b
fit c*x 'recurs1.1.dat' using 0:($1/1000) via c
fit d*x 'recurs2.1.dat' using 0:($1/1000) via d

set print "fit_values.txt"
print sprintf("a = %.4f ns/count", a)
print sprintf("b = %.4f ns/count", b)
print sprintf("c (2 nstep, E_1) = %.4f ns/count", c)
print sprintf("d (2 nstep, E_2) = %.4f ns/count", d)
print sprintf("E_1 = %.4f meV", 11.740818)
print sprintf("E_2 = %.4f meV", 11.947426)
print sprintf("T (nstep fits) = %.4f K", (11.947426-11.740818)/(0.08617718*log(b/a)))
print sprintf("T (2 nstep fits) = %.4f K", (11.947426-11.740818)/(0.08617718*log(d/c)))
set print

set key box opaque

set style line 1 linecolor rgb "light-green" lw 3 pt 7 ps 0.6
set style line 2 linecolor rgb "pink" lw 3 pt 7 ps 0.6
set style line 3 linecolor rgb "dark-green" lw 3 pt 7 ps 0.6
set style line 4 linecolor rgb "red" lw 3 pt 7 ps 0.6
set style line 5 linecolor rgb "blue" lw 3 pt 7 ps 0.6
set style line 6 linecolor rgb "dark-violet" lw 3 pt 7 ps 0.6
set style line 7 linecolor rgb "web-green" lw 3 pt 7 ps 0.6
set style line 8 linecolor rgb "orange" lw 3 pt 7 ps 0.6

set output "comparison.pdf"
set xrange [0:*]
plot 'recurs1.dat' using 0:($1/1000) w p ls 1 title 'E_1=11.740818 meV', \
     'recurs2.dat' using 0:($1/1000) w p ls 2 title 'E_2=11.947426 meV', \
     a*x w l ls 3 title sprintf('Fit: %.4f x', a), \
     b*x w l ls 4 title sprintf('Fit: %.4f x', b)

set output "comparison_2nstep.pdf"
set xrange [0:*]
plot 'recurs1.1.dat' using 0:($1/1000) w p ls 7 title 'E_1=11.740818 meV, 2 nstep', \
     'recurs2.1.dat' using 0:($1/1000) w p ls 8 title 'E_2=11.947426 meV, 2 nstep', \
     c*x w l ls 3 title sprintf('Fit E_1: %.4f x', c), \
     d*x w l ls 4 title sprintf('Fit E_2: %.4f x', d)

set output "convergence.pdf"
set xrange [0:*]
set label 1 sprintf("T (nstep) = %.3f K\nT (2 nstep) = %.3f K", \
    (11.947426-11.740818)/(0.08617718*log(b/a)), \
    (11.947426-11.740818)/(0.08617718*log(d/c))) \
    at graph 0.85,0.15 right front boxed
plot 'recurs1.1.dat' using 0:($1/1000) w p ls 7 title 'E_1, 2 nstep', \
     'recurs2.1.dat' using 0:($1/1000) w p ls 8 title 'E_2, 2 nstep', \
     'recurs1.dat' using 0:($1/1000) w p ls 1 title 'E_1, nstep', \
     'recurs2.dat' using 0:($1/1000) w p ls 2 title 'E_2, nstep', \
     c*x w l ls 5 title sprintf('E_1, 2 nstep fit: %.4f x', c), \
     [0:200] d*x w l ls 6 title sprintf('E_2, 2 nstep fit: %.4f x', d), \
     [0:120] a*x w l ls 3 title sprintf('E_1, nstep fit: %.4f x', a), \
     [0:100] b*x w l ls 4 title sprintf('E_2, nstep fit: %.4f x', b)
unset label 1

unset output
