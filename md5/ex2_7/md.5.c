#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define twopi 6.28318530718
#define kB 0.08617718  /*  kB= 1./11.604=0.086 meV/K  */
/* UNITS: meV, ps, Ang -- mass is in units of ~1.602e-26 kg ~= 9.648 amu  */

int N;             //number of atoms
char *atomtype;    //atomic species (1 char)
double dt,halfdt;  //timestep and half of it
double mass;       //mass
double kspring;    //spring constant
double d0;         //equilibrium spring distance
double side,halfside;//side of the box
long int nsteps;   //number of simulation steps
long int nstep;    //actual step, changing during simulation
long int print;    //print every these steps
double *x,*y,*z,*Vx,*Vy,*Vz,*Fx,*Fy,*Fz;
double *xprev,*yprev,*zprev,*xtmp,*ytmp,*ztmp; // to store the old coordinates for Verlet
double epot,ekin,etot,Tm,Tin;
double gam; // gam in reciprocal time units, ps^-1  (F = -m*v*gam)
char inputxyz[30],trajfile[30];
FILE *ftraj;

double square(double val){ return val*val; }
double cube(double val){ return val*val*val; }

void read_parameters(char *filename){
	char dummy[100];
	FILE *fin;
	void error() { fprintf(stderr,"ERROR in input parameters file %s\n",filename); exit(1); }

	if((fin=fopen(filename,"r"))==NULL) error();
	if( fscanf(fin,"%s %ld ", dummy, &nsteps  ) != 2 ) error();
	if( fscanf(fin,"%s %ld ", dummy, &print   ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &mass    ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &dt      ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &kspring ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &d0      ) != 2 ) error();
	if( fscanf(fin,"%s %s  ", dummy, inputxyz ) != 2 ) error();
	if( fscanf(fin,"%s %s  ", dummy, trajfile ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &side    ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &Tin     ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &gam     ) != 2 ) error();

	//printout parameters to check
	fprintf(stderr,"nsteps %ld\n" ,nsteps);
	fprintf(stderr,"print %ld\n"  ,print);
	fprintf(stderr,"mass %g\n"    ,mass);
	fprintf(stderr,"timestep %g\n",dt);
	fprintf(stderr,"kspring %g\n" ,kspring);
	fprintf(stderr,"d0 %g\n"      ,d0);
	fprintf(stderr,"inputxyz %s\n",inputxyz);
	fprintf(stderr,"trajfile %s\n",trajfile);
	fprintf(stderr,"side %g\n"    ,side);
	fprintf(stderr,"temperature %g\n",Tin);
	fprintf(stderr,"gamma %g\n"   ,gam);

	fclose(fin);
	halfside=side/2.;
	halfdt=dt/2.;
}

void readxyz(){
	int i,n=0,readCharCount;
	char dummy[300];
	FILE *fin;
	void error() { fprintf(stderr,"ERROR in input structure file %s, %c %g %g %g\n",inputxyz,atomtype[i],x[i],y[i],z[i]); exit(1); }

	if((fin=fopen(inputxyz,"r"))==NULL) error();

	//the first line should contain the number of atoms
	fgets(dummy,sizeof(dummy),fin);
	if( sscanf(dummy,"%d",&N) != 1 ) error();

	if( (x=(double *)  calloc(N, sizeof(double))) == NULL ) n++;
	if( (y=(double *)  calloc(N, sizeof(double))) == NULL ) n++;
	if( (z=(double *)  calloc(N, sizeof(double))) == NULL ) n++;
	if( (xprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (yprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (zprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (xtmp=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (ytmp=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (ztmp=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vx=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vy=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vz=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Fx=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Fy=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Fz=(double *)  calloc(N, sizeof(double))) == NULL ) n++;
	if( (atomtype=(char *) calloc(N,sizeof(char))) == NULL ) n++;
	if(n>0){fprintf(stderr,"Allocation error %d\n",n); exit(1);}

	// the second line is a comment, we skip it
	fgets(dummy,sizeof(dummy),fin);

	for(i=0;i<N;i++){
		//read positions
		fgets(dummy,sizeof(dummy),fin);
		if( sscanf(dummy,"%s %lf %lf %lf%n", &atomtype[i],&x[i],&y[i],&z[i],&readCharCount) !=4 ) error();
		//read velocities if present, otherwise set them to zero
		if( sscanf(dummy+readCharCount,"%lf %lf %lf", &Vx[i],&Vy[i],&Vz[i]) !=3 ) Vx[i]=Vy[i]=Vz[i]=0.;
	}
	fclose(fin);
	fprintf(stderr,"Simulation with %d atoms\n",N);
}

double distx(int i, int j){
	double dX= x[i]-x[j];
	return dX-round(dX/side)*side;
}

double disty(int i, int j){
	double dY= y[i]-y[j];
	return dY-round(dY/side)*side;
}

double distz(int i, int j){
	double dZ= z[i]-z[j];
	return dZ-round(dZ/side)*side;
}

double dist(int i, int j){
	return sqrt( square(distx(i,j)) + square(disty(i,j)) + square(distz(i,j)) );
}

void calc_forces(){
	int i; double effe;
	//reset the forces
	for (i=0;i<N;i++) { Fx[i]=Fy[i]=Fz[i]=0; }

	//chain
	for (i=0;i<N-1;i++){
		    effe = -kspring*(dist(i,i+1) - d0)/dist(i,i+1);
		    Fx[i] += effe*distx(i,i+1); Fx[i+1] += -effe*distx(i,i+1);
		    Fy[i] += effe*disty(i,i+1); Fy[i+1] += -effe*disty(i,i+1);
		    Fz[i] += effe*distz(i,i+1); Fz[i+1] += -effe*distz(i,i+1);
	}
}

void calc_energies(){
	int i;

	ekin=0;
	for(i=0;i<N;i++){ ekin+=( Vx[i]*Vx[i] + Vy[i]*Vy[i] + Vz[i]*Vz[i] ); }

	ekin *= 0.5*mass;
	Tm = 2*ekin/(3.*N*kB);

	epot=0;
	//chain
	for (i=0;i<N-1;i++)
		epot += 0.5*kspring*square( dist(i,i+1) - d0 );

	etot=ekin+epot;
}


double Fgauss(){ //gaussian-distributed random force
	static int once=1,i=0;
	static double gen1,gen2,var;
	if(once){
		once=0;
		srand(4567);
		var=2*gam*kB*Tin/(dt/2); //velocity advances in steps of dt/2
	}
	if(var==0.) return 0.0;
	if(i==0){
		gen1=(rand()+1.)/(RAND_MAX+1.0);
		gen2=(rand()+1.)/(RAND_MAX+1.0);
		gen1=sqrt(-2.*log(gen1)*var);
		gen2=twopi*gen2;
		i=1;
		return gen1*cos(gen2);
	}
	else{
	    i=0;
	    return gen1*sin(gen2);
	}
}

void vVerlet_Langevin(){
	int i; static int once=1;
	if(once){ once=0; calc_forces(); }

	// update velocities(1)
	for (i=0;i<N;i++){
		Vx[i] += ( Fx[i] - gam*Vx[i] + Fgauss() )/mass*halfdt;
		Vy[i] += ( Fy[i] - gam*Vy[i] + Fgauss() )/mass*halfdt;
		Vz[i] += ( Fz[i] - gam*Vz[i] + Fgauss() )/mass*halfdt;
	}

	//update positions
	for (i=0;i<N;i++){
		x[i] += Vx[i]*dt;
		y[i] += Vy[i]*dt;
		z[i] += Vz[i]*dt;
	}

	calc_forces(); // update the forces

	// update velocities(2)
	for (i=0;i<N;i++){
		Vx[i] = ( Vx[i] + (Fx[i] + Fgauss())/mass*halfdt )/(1. + gam/mass*halfdt);
		Vy[i] = ( Vy[i] + (Fy[i] + Fgauss())/mass*halfdt )/(1. + gam/mass*halfdt);
		Vz[i] = ( Vz[i] + (Fz[i] + Fgauss())/mass*halfdt )/(1. + gam/mass*halfdt);
	}
}

void write_traj(){
	static int once=1;
	int i;
	if(once){
		once=0;
		if((ftraj=fopen(trajfile,"w"))==NULL) { fprintf(stderr,"ERROR while saving trajectory file\n"); exit(1); }
	}

	fprintf(ftraj,"%d\nLattice=\"%g 0.0 0.0  0.0 %g 0.0  0.0 0.0 %g\"\tTIME: %g\n",N,side,side,side,dt*nstep);
	for(i=0;i<N;i++){
		fprintf(ftraj,"%c\t%.6f\t%.6f\t%.6f\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%6e\n" ,atomtype[i],x[i],y[i],z[i],Vx[i],Vy[i],Vz[i],Fx[i],Fy[i],Fz[i]);
	}
	fflush(ftraj);
}

void printout(){
	static int once=1;
	if(once){
		once=0;
		printf("#time\tekin\t\tepot\t\tetot\t\tchain_length\ttemperature\n");
	}
	calc_energies();
	printf("%g\t%lf\t%lf\t%lf\t%lf\t%lf\n",dt*nstep,ekin,epot,etot,dist(0,N-1),Tm );
	write_traj();
	fflush(stdout);
}


int main(int argc, char *argv[])
{
   // read parameter file
   if (argc < 2) {fprintf(stderr,"Provide parameter file\n"); exit(1);}
   read_parameters(argv[1]);

   // read initial conditions
   readxyz();

   // write trajectory and print some information
   printout();

   // dynamics loop
   for (nstep=1;nstep<=nsteps;nstep++)
   {

      vVerlet_Langevin(); // one time of dynamics

      // write trajectory and print some information
      if ( nstep % print == 0 ) printout();
   }

   fclose(ftraj);

   return 0;
}


