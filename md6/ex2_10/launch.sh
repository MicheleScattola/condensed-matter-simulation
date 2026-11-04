#!/bin/bash

./md.x param_relaxed > relaxation.dat
tail -n 58 traj.xyz > tmp.xyz
touch 1PGB_relaxed.xyz
awk '{print $1,$2,$3,$4}' tmp.xyz > 1PGB_relaxed.xyz
rm tmp.xyz
./md.x param_unfolded > unfolding.dat
tail -n 58 traj_unfolded.xyz > tmp.xyz
touch 1PGB_unfolded.xyz
awk '{print $1,$2,$3,$4}' tmp.xyz > 1PGB_unfolded.xyz
rm tmp.xyz
./md.x param_refolded > refolding.dat
tail -n 58 traj_refolded.xyz > tmp.xyz
touch 1PGB_refolded.xyz
awk '{print $1,$2,$3,$4}' tmp.xyz > 1PGB_refolded.xyz
rm tmp.xyz