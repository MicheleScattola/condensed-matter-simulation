```bash
mike@thinkpad:ex2_3$ time ./md.x param 1.5 > out1.5.dat && time ./md.x param 2.0 > out2.0.dat && time ./md.x param 3.0 > out3.0.dat
nsteps 100000
print 1000
mass 100
timestep 0.001
kspring 10
d0 2
inputxyz initialchain.w.xyz
trajfile /dev/null
c_rep 0.01
side 10
kbox 0

All parameters read from file
Using CUTOFF radius: 1.50
Simulation with 152 atoms

real    0m9,454s
user    0m9,442s
sys     0m0,005s
nsteps 100000
print 1000
mass 100
timestep 0.001
kspring 10
d0 2
inputxyz initialchain.w.xyz
trajfile /dev/null
c_rep 0.01
side 10
kbox 0

All parameters read from file
Using CUTOFF radius: 2.00
Simulation with 152 atoms

real    0m9,781s
user    0m9,770s
sys     0m0,002s
nsteps 100000
print 1000
mass 100
timestep 0.001
kspring 10
d0 2
inputxyz initialchain.w.xyz
trajfile /dev/null
c_rep 0.01
side 10
kbox 0

All parameters read from file
Using CUTOFF radius: 3.00
Simulation with 152 atoms

real    0m11,212s
user    0m11,202s
sys     0m0,002s
```