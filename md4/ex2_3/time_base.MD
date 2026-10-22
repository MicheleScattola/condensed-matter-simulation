```bash
mike@thinkpad:ex2_3$ time ./md.x param > out1.5.dat && time ./md.x param > out2.0.dat && time ./md.x param > out3.0.dat
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
Using CUTOFF radius: 100000.00
Potential shift: 0.00
Simulation with 152 atoms

real    0m26,132s
user    0m25,996s
sys     0m0,020s
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
Using CUTOFF radius: 100000.00
Potential shift: 0.00
Simulation with 152 atoms
   
real    0m24,830s
user    0m24,790s
sys     0m0,015s
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
Using CUTOFF radius: 100000.00
Potential shift: 0.00
Simulation with 152 atoms

real    0m24,955s
user    0m24,919s
sys     0m0,011s
```
