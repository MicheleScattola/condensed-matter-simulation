#!/bin/bash

#./md.x parameters.7 4567 19 > out19.dat
#./md.x parameters.7 4877 21 > out21.dat 

paste out19.dat out21.dat | awk 'NR>10{print $4,$9}' > cv.dat
awk 'NR>10{$(NF+1) = sqrt(($2-$1)^2)/2; print}' cv.dat > tmp.dat
mv tmp.dat cv.dat # cv is now column $3


for i in 19 21; do
    # progressive $1 = <E> , $2 = <E^2>
    awk 'NR>10{a=a+$4; b=b+($4*$4); n=n+1; print $1,a/n,b/n}' out$i.dat > ave$i.dat
    
    # add cv column
    paste ave$i.dat cv.dat > ave${i}_tmp.dat # cv is now column $6
    mv ave${i}_tmp.dat ave$i.dat
    awk -v temp="$i" 'BEGIN{k=0.08617718; t=temp*temp} {print $1,($3-$2*$2)/(k*t*$6)}' ave$i.dat > h$i.dat
done

