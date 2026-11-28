set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "equilibration.pdf"
set key box opaque bottom right offset 0,2
set style line 1 linecolor rgb "web-green" linewidth 2
set style line 2 linecolor rgb "red" linewidth 2 dt 4

set logscale y
#set xrange [0.2:*]

set ylabel "h(t)"
set xlabel "t [ns]"
set ytics nomirror

set multiplot layout 1,2 margins 0.15, 0.95, 0.15, 0.9 spacing 0, 0

a=1.0
b=0.7
f(x) = 1 + a*exp(-x/b)
fit [0.2:] f(x) "h9.dat" u (($1)/1000):2 via a,b
set title sprintf("Equilibration time at 9K: {/Symbol t} = %.2f ns",b)
p "h9.dat" u (($1)/1000):2  w l ls 1 t "T = 9K", [0.2:] f(x) w l ls 2 t sprintf("Fit: 1+%.1f*e^{(-t/%.2f)}",a,b)

unset ylabel
unset ytics
set xlabel "t [ns]"

c=1.0
d=0.7
g(x) = 1 + c*exp(-x/d)
fit [0.2:] g(x) "h11.dat" u (($1)/1000):2 via c,d
set title sprintf("Equilibration time at 11K: {/Symbol t} = %.2f ns",d)
p "h11.dat" u (($1)/1000):2  w l ls 1 t "T = 11K", [0.2:] f(x) w l ls 2 t sprintf("Fit: 1+%.1f*e^{(-t/%.2f)}",c,d)

unset multiplot
set out
set term pop