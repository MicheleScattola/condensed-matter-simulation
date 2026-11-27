set term pdfcairo enhanced color rounded size 13,5 font "Arial,16"
set output "cutoff_comparison.pdf"
set border lw 1.2
set style line 1 linecolor rgb "web-green" lw 2
set style line 2 linecolor rgb "orange" lw 2
set style line 3 linecolor rgb "red" lw 2
set xlabel "Time [ps]"
set key bottom right box opaque  offset 0, 1.5
set yrange [501.073:501.12]

set multiplot layout 1,3 margins 0.1, 0.95, 0.15, 0.9 spacing 0.0,0.0
set ylabel "Total energy [meV]"
set title "Cutoff = 1.5 A"
plot "out1.5.dat" using 1:4 with lines ls 1 title "No cutoff", \
     "1.5.dat" using 1:4 with lines ls 2 title "Cutoff", \
     "shift1.5.dat" using 1:4 with lines ls 3 title "Cutoff + shift"

set key center right box opaque

#set yrange [501.105:501.12]
unset ylabel
unset ytics

set title "Cutoff = 2.0 A"
plot "out2.0.dat" using 1:4 with lines ls 1 t "No cutoff", \
     "2.0.dat" using 1:4 with lines ls 2 t "Cutoff", \
     "shift2.0.dat" using 1:4 with lines ls 3 t "Cutoff + shift"

set title "Cutoff = 3.0 A"
plot "out3.0.dat" using 1:4 with lines ls 1 t "No cutoff", \
     "3.0.dat" using 1:4 with lines ls 2 t "Cutoff", \
     "shift3.0.dat" using 1:4 with lines ls 3 t "Cutoff + shift"
unset multiplot
set output