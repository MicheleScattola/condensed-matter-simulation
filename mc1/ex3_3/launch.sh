#!/bin/bash

export LC_NUMERIC=C

rm -f *.dat
make

echo -e "\nRUN 1 in progress...\n"
./md.x mdparams > md_transient.dat
./mc.x mcparams > mc_transient.dat

cp md_transient.dat md_full.dat
cp mc_transient.dat mc_full.dat

for i in {2..10};do
  echo -e "\nRUN $i in progress...\n"
  ./md.x mdparams | awk 'NR > 1' >> md_full.dat
  ./mc.x mcparams | awk 'NR > 1' >> mc_full.dat

done

awk -v col=7 -v tmin=100 -v tmax=1e7 -f ./1Dbinning.awk md_full.dat > mdout.dat
awk -v col=8 -v tmin=1e5 -v tmax=1e7 -f ./1Dbinning.awk mc_full.dat > mcout.dat

gnuplot plot.gnuplot