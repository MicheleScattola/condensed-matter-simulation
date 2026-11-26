#!/bin/bash
LC_NUMERIC=C
lammps -in lammps1.in | tee serial.out
> data.dat
baseline=$(grep -oP 'Loop time of \s*\K[\d.]+' serial.out | sed -n 2p)
printf "%s\t%s\n" "1" "1.0" >> data.dat
for i in {1..10}; do

    mpiexec -n $i lammps -in lammps1.in | tee parallel$i.out
    time=$(grep -oP 'Loop time of \s*\K[\d.]+' parallel$i.out | sed -n 2p)
    awk -v cpu="$i" -v t0="$baseline" -v t1="$time" 'BEGIN { printf "%s\t%s\n", cpu, t0/t1 }' >> data.dat
done

lammps -in lammps2.in | tee serial10.out
> data10.dat
baseline10=$(grep -oP 'Loop time of \s*\K[\d.]+' serial10.out | sed -n 2p)
printf "%s\t%s\n" "1" "1.0" >> data10.dat
for i in {1..10}; do

    mpiexec -n $i lammps -in lammps2.in | tee parallel_10_$i.out
    time=$(grep -oP 'Loop time of \s*\K[\d.]+' parallel_10_$i.out | sed -n 2p)
    awk -v cpu="$i" -v t0="$baseline10" -v t1="$time" 'BEGIN { printf "%s\t%s\n", cpu, t0/t1 }' >> data10.dat
done

gnuplot -e "filename"="'data.dat'" -e "outname"="'perf.pdf'" -e "titlename"="'Speedup'" plot.gnuplot
gnuplot -e "filename"="'data10.dat'" -e "outname"="'perf10.pdf'" -e "titlename"="'Speedup x10 atoms'" plot.gnuplot
