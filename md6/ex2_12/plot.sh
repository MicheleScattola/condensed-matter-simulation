echo "set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font 'Arial,28'
set border lw 1.5 front
set out 'unfolding.pdf'

set style line 1 linecolor rgb 'purple' linewidth 1;
set style line 2 linecolor rgb 'turquoise' linewidth 1;
set style line 3 linecolor rgb 'navy' linewidth 1;
set style line 4 linecolor rgb 'red' linewidth 3;

set title 'Unfolding at 450K - 10 runs';
set xlabel 't [ps]';
set ylabel 'q';
set key top right;
set yrange [0:1];
p 'unfolding_4.dat' u 1:2 w l ls 1 t 'data 4', 'unfolding_7.dat' u 1:2 w l ls 2 t 'data 7', 'unfolding_2.dat' u 1:2 w l ls 3 t 'data 2', 'average_unfolding.dat' u 1:2 w l ls 4 t 'Average';
set out
set term pop" | gnuplot

echo "set term push
set term pdfcairo enhanced color rounded size 12,6 lw 1.5 font 'Arial,28'
set border lw 1.5 front
set out 'refolding.pdf'

set style line 1 linecolor rgb 'purple' linewidth 1;
set style line 2 linecolor rgb 'turquoise' linewidth 1;
set style line 3 linecolor rgb 'navy' linewidth 1;
set style line 4 linecolor rgb 'red' linewidth 3;

set title 'Refolding at 10K - 10 runs';
set xlabel 't [ps]';
set ylabel 'q';
set key bottom right;
set yrange [0:1];
p 'refolding_4.dat' u 1:2 w l ls 1 t 'data 4', 'refolding_7.dat' u 1:2 w l ls 2 t 'data 7', 'refolding_2.dat' u 1:2 w l ls 3 t 'data 2', 'average_refolding.dat' u 1:2 w l ls 4 t 'Average';
set out
set term pop" | gnuplot