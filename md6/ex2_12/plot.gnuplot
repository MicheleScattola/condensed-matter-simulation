set term push
set term pdfcairo enhanced color rounded size 8,6 lw 1.5 font 'Arial,28'
set border lw 1.5 front
set key box opaque

# 10 run average

set style line 1 linecolor rgb 'purple' lw 2 dt 2
set style line 2 linecolor rgb 'turquoise' lw 2 dt 2
set style line 3 linecolor rgb 'navy' lw 2 dt 2
set style line 4 linecolor rgb 'red' lw 3


set out 'unfolding.pdf'
set title 'Unfolding at 450K - 10 runs'
set xlabel 't [ps]'
set ylabel 'q'
set key top right
set yrange [0:1]
p 'unfolding_4.dat' u 1:2 w l ls 1 t 'data 4', 'unfolding_7.dat' u 1:2 w l ls 2 t 'data 7', 'unfolding_2.dat' u 1:2 w l ls 3 t 'data 2', 'average_unfolding.dat' u 1:2 w l ls 4 t 'Average'


set out 'refolding.pdf'
set title 'Refolding at 10K - 10 runs'
set xlabel 't [ps]'
set ylabel 'q'
set key bottom right
set yrange [0:1]
p 'refolding_4.dat' u 1:2 w l ls 1 t 'data 4', 'refolding_7.dat' u 1:2 w l ls 2 t 'data 7', 'refolding_2.dat' u 1:2 w l ls 3 t 'data 2', 'average_refolding.dat' u 1:2 w l ls 4 t 'Average'

# fit
set out 'single_exp.pdf'

set style line 1 linecolor rgb 'web-green' lw 3 dt 1
set style line 2 linecolor rgb 'red' lw 3 dt 4

set ylabel 'q'
set yrange [0:1]
set xlabel 't[ps]'

f(x)=a*(1-exp(-x/b))+c
a=1.0
b=0.1
c=0.0
fit f(x) 'average_refolding.dat' u 1:2 via a,b,c
kba = a/b
kab = (1-a)/b
set title sprintf('Refolding at 10K: k_{ba}=%.2e , k_{ab}=%.2e',kba,kab)
p 'average_refolding.dat' w l ls 1 t 'Refolding average', f(x) w l ls 2 title sprintf('Exp fit: {/Symbol t}=%.1f ps',b)

# loglin fit

set out 'double_exp.pdf'
set key top left
set ylabel 'q'
set yrange [0:1]
set xlabel 't [ps]'
set logscale x

A1=0.2
tau1=10.0
A2=0.6
tau2=200.0
C=0.08

F(x) = A1*(1-exp(-x/tau1)) + A2*(1-exp(-x/tau2)) + C

fit [1:] F(x) 'average_refolding.dat' u 1:2 via A1, tau1, A2, tau2, C

set title sprintf('Double exp sum: {/Symbol t}_1=%.1f ps , {/Symbol t}_2=%.1f ps',tau1,tau2)

plot 'average_refolding.dat' w l ls 1 t 'Refolding average', \
     [1:] F(x) w l ls 2 t 'Double Exp fit'

set out
set term pop