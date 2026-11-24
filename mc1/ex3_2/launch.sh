#!/bin/bash

export LC_NUMERIC=C

rm -f *.dat
make

# progressive gamma
for i in {1..6}; do
    gam=0.$i

    ./mc.x parameters.1 "$gam" | awk 'NR > 1 { t += $7; print $6,t,$1 }' > "out.$gam.dat"
    awk -v gam="$gam" '$1 >= 100.0 { print gam, $2,$3/$2; exit }' "out.$gam.dat" >> data.dat
done

source plot.sh