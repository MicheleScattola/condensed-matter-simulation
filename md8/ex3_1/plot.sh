echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "2Dplots.pdf"

set multiplot layout 1,2 margins 0.1, 0.9, 0.1, 0.9 spacing 0.2, 0

set cblabel "p(q,R_{g}) [meV]" rotate by 0 offset -5.5, 9.5
set xlabel "head-tail distance q [A]"
set ylabel "gyration radius R_{g} [A]"
p "prob.dat" w image notitle

set cblabel "F(q,R_{g}) [meV]"
set xlabel "head-tail distance q [A]"
set ylabel "gyration radius R_{g} [A]"
p "free.dat" w image notitle

unset multiplot

set out
set term pop' | gnuplot