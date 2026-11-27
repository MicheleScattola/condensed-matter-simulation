set term pdfcairo enhanced color rounded size 9,5 font "Arial,18"
set border lw 1.5
set output "comparison.pdf"

a = 1.0
b = 1.0
fit a*x 'recurs1.dat' using 0:($1/1000) via a
fit b*x 'recurs2.dat' using 0:($1/1000) via b
temp = (11.947426-11.740818)/(0.08617718*log(b/a))
stats 'out1.dat' every ::10 using 2 nooutput
t1 = STATS_mean/(3./2.*12*0.0862)
stats 'out2.dat' every ::10 using 2 nooutput
t2 = STATS_mean/(3./2.*12*0.0862)

set key box opaque

set style line 1 linecolor rgb "web-green" lw 2 pt 7 ps 0.6
set style line 2 linecolor rgb "orange" lw 2 pt 7 ps 0.6
set style line 3 linecolor rgb "dark-green" lw 2 pt 7 ps 0.6
set style line 4 linecolor rgb "red" lw 2 pt 7 ps 0.6

set multiplot layout 1,2 spacing 1, 0

set xlabel 'Recursion count'
set ylabel 'Mean recursion time [ns]'
set title 'Recursion T'
set label 1 sprintf("T = %.3f K",temp) at graph 0.85,0.15 right front boxed
plot 'recurs1.dat' using 0:($1/1000) w p ls 1 title 'E_1=11.740818 meV', \
     'recurs2.dat' using 0:($1/1000) w p ls 2 title 'E_2=11.947426 meV', \
     a*x w l ls 3 title sprintf('Fit: %.4f x', a), \
     b*x w l ls 4 title sprintf('Fit: %.4f x', b)
unset label 1

set yrange [0:15]
set xlabel 'Time [ns]'
set ylabel 'Kinetic energy [meV]'
set title 'Equipartition T = <E_k>/(3/2 N K_B)'
set label 1 sprintf("T = %.3f K",t1) at graph 0.85,0.15 right front boxed
plot 'out1.dat' u (($1)/1000):2 w l ls 1 t 'E_1=11.740818 meV'

unset multiplot
set out
set term pop
