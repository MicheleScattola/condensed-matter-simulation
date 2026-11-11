#!/bin/bash

export LC_NUMERIC=C

rm -f ave*.dat cv.dat h*.dat
make
./md.x parameters.7 4567 19 > out19.dat
./md.x parameters.7 4877 21 > out21.dat 

# progressive $1 = <E> , $2 = <E^2>
for i in 19 21; do
    awk 'NR>10{a=a+$4; b=b+($4*$4); n=n+1; print $1,a/n,b/n}' out$i.dat > ave$i.dat
done    
# cv calculation
paste ave19.dat ave21.dat | awk '{print ($5 - $2) / 2.0}' > cv.dat

for i in 19 21; do

    paste ave$i.dat cv.dat > tmp.dat # cv is now column $4
    mv tmp.dat ave$i.dat
    awk -v temp="$i" 'BEGIN{k=0.08617718; t=temp*temp} {print $1,($3-$2*$2)/(k*t*$4)}' ave$i.dat > h$i.dat
    
done

source ./plot.sh