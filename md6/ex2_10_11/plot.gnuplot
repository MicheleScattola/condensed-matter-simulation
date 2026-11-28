set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font "Arial,28"
set border lw 1.5 front
set out "order_parameters.pdf"

set multiplot layout 1,2 margins 0.1, 0.9, 0.15, 0.9 spacing 0, 0

set style line 1 linecolor rgb "red" linewidth 2
set style line 2 linecolor rgb "blue" linewidth 2

set yrange [0:1]
set y2range [0:25]

set ylabel "q"
set xlabel "t [ps]"
set ytics nomirror
set title "Unfolding at 450K"
unset y2tics
unset y2label

p "unfolding.dat" u 1:7 w l ls 1 t "q_{unfold}", "" u 1:8 w l ls 2 t "dRMSD_{unfold}" axes x1y2

unset ylabel
unset ytics
set y2tics nomirror
set y2label "dRMSD"
set xlabel "t [ps]"
set title "Folding at 10K"
set key bottom right

p "refolding.dat" u ($1+200):7 w l ls 1 t "q_{refold}", "" u ($1+200):8 w l ls 2 t "dRMSD_{refold}" axes x1y2

unset multiplot
set out
set term pop