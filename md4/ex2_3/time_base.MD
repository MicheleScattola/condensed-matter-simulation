```bash
michele.scattola@lab12:/home/unimi/roberto.guerra/condivisa/michele.scattola/condensed-matter-simulation/md4/ex2_3$ time ./md.x param > out1.5.dat && time ./md.x param > out2.0.dat && time ./md.x param > out3.0.dat
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

real    0m45.586s
user    0m45.474s
sys     0m0.001s
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

real    0m45.382s
user    0m45.301s
sys     0m0.002s
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

real    0m45.588s
user    0m45.501s
sys     0m0.001s
```
