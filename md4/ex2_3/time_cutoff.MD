```bash
mike@thinkpad:ex2_3$ time ./md.x param 1.5 0 > 1.5.dat && time ./md.x param 1.5 1 > shift1.5.dat && time ./md.x param 2.0 0 > 2.0.dat && time ./md.x param 2.0 1 > shift2.0.dat && time ./md.x param 3.0 0 > 3.0.dat && time ./md.x param 3.0 1 > shift3.0.dat
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
Potential shift: 0.000000
Simulation with 152 atoms

real    0m10,223s
user    0m10,212s
sys     0m0,006s
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
Potential shift: 0.000173
Simulation with 152 atoms

real    0m9,582s
user    0m9,576s
sys     0m0,004s
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
Potential shift: 0.000000
Simulation with 152 atoms

real    0m9,964s
user    0m9,950s
sys     0m0,003s
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
Potential shift: 0.000010
Simulation with 152 atoms

real    0m10,152s
user    0m10,130s
sys     0m0,011s
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
Potential shift: 0.000000
Simulation with 152 atoms

real    0m11,341s
user    0m11,330s
sys     0m0,006s
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
Potential shift: 0.000000
Simulation with 152 atoms

real    0m11,138s
user    0m11,123s
sys     0m0,006s
```