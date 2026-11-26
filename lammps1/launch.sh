#!/bin/bash
LC_NUMERIC=C
lammps -in lammps.in | tee serial.out
> data.dat
baseline=$(grep -oP 'Loop time of \s*\K[\d.]+' serial.out | sed -n 2p)
printf "%s\t%s\n" "1" "1.0" >> data.dat
for i in {1..10}; do

    mpiexec -n $i lammps -in lammps.in | tee parallel$i.out
    time=$(grep -oP 'Loop time of \s*\K[\d.]+' parallel$i.out | sed -n 2p)
    awk -v cpu="$i" -v t0="$baseline" -v t1="$time" 'BEGIN { printf "%s\t%s\n", cpu, t0/t1 }' >> data.dat
done

gnuplot -e "filename"="'data.dat'" -e "outname"= plot.gnuplot