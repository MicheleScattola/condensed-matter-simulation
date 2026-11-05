#!/bin/bash

export LC_ALL=C

# create relaxed state
./md.x param_relaxed | awk '{print $1,$7}' > relaxation.dat
tail -n 58 traj.xyz > tmp.xyz
touch 1PGB_relaxed.xyz
touch 1PGB_unfolded.xyz
touch 1PGB_refolded.xyz
awk '{print $1,$2,$3,$4}' tmp.xyz > 1PGB_relaxed.xyz
rm tmp.xyz traj.xyz


# functions for unfolding and refolding
unfolding(){
    ./md.x param_unfolded | awk '{print $1,$7}' > $1
    tail -n 58 traj_unfolded.xyz > tmp.xyz
    awk '{print $1,$2,$3,$4}' tmp.xyz > 1PGB_unfolded.xyz
    rm tmp.xyz traj_unfolded.xyz
    paste unfolding.dat $1 > tmp.dat
    awk '{print $1,$2+$4}' tmp.dat > unfolding.dat
    rm tmp.dat
}

refolding(){
    ./md.x param_refolded | awk '{print $1,$7}' > $1
    # refolding doesnt need to save its final xyz state
    paste refolding.dat $1 > tmp.dat
    awk '{print $1,$2+$4}' tmp.dat > refolding.dat
    rm tmp.dat
}

# run 1
echo -e "\n[RUN 1] unfolding at 450K...\n"
./md.x param_unfolded | awk '{print $1,$7}' > unfolding.dat
tail -n 58 traj_unfolded.xyz > tmp.xyz
awk '{print $1,$2,$3,$4}' tmp.xyz > 1PGB_unfolded.xyz
rm tmp.xyz traj_unfolded.xyz

echo -e "\n[RUN 1] folding at 10K...\n"
./md.x param_refolded | awk '{print $1,$7}' > refolding.dat


for i in {2..10}; do
    echo -e "\n[RUN $i] unfolding at 450K...\n"
    unfolding unfolding_$i.dat
    echo -e "\n[RUN $i] folding at 10K...\n"
    refolding refolding_$i.dat
done

# compute averages
awk '{print $1,$2/10}' unfolding.dat > average_unfolding.dat
awk '{print $1,$2/10}' refolding.dat > average_refolding.dat

source ./plot.sh