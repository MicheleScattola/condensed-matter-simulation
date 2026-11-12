echo 'set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "temp.pdf"

set multiplot layout 1,2 margins 0.15, 0.95, 0.15, 0.9 spacing 0, 0

p "prob.dat" w image
p "free.dat" w image
unset multiplot

set out
set term pop' | gnuplot