set term pdfcairo enhanced color rounded size 7,5 font "Arial,22"
set border lw 1.5 front
set output "decay.pdf"

set style line 1 linecolor rgb "dark-violet" lw 2 dt 3
set style line 2 linecolor rgb "orange" lw 2 dt 3
set style line 3 linecolor rgb "violet" lw 2 dt 3
set style line 4 linecolor rgb "red" lw 3

set title '10 seed average - Temperature';
set xlabel 't [ps]';
set ylabel 'T [K]';
set key bottom right;
p 'out_4.dat' u 1:2 w l ls 1 t 'data 4', \
'out_7.dat' u 1:2 w l ls 2 t 'data 7', \
'out_2.dat' u 1:2 w l ls 3 t 'data 2', \
'average.dat' u 1:2 w l ls 4 t 'Average';

set output