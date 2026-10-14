#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define square(x) ((x)*(x))
#define cube(x) ((x)*(x)*(x))
#define kB 0.08617718  /*  kB= 1./11.604=0.086 meV/K  */
/* UNITS: meV, ps, Ang -- mass is in units of ~1.602e-26 kg ~= 9.648 amu  */

int N;             //number of atoms
char *atomtype;    //atomic species (1 char)
double dt;         //timestep
double mass;       //mass
double kspring;    //spring constant
double d0;         //equilibrium spring distance
long int nsteps;   //number of simulation steps
long int nstep;    //actual step, changing during simulation
long int print;    //print every these steps
double *x,*y,*z,*Vx,*Vy,*Vz,*Fx,*Fy,*Fz;
double *xprev,*yprev,*zprev,*xtmp,*ytmp,*ztmp; // to store the old coordinates for Verlet
double *x0,*yy0,*z0,*Vx0,*Vy0,*Vz0; //initial coords and velocities (y0 is bessel function in mathlib)
double epot,ekin,etot;
char inputxyz[30],trajfile[30],recursfile[30];
FILE *ftraj,*frec;


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
	if( fscanf(fin,"%s %s " , dummy, inputxyz ) != 2 ) error();
	if( fscanf(fin,"%s %s " , dummy, trajfile ) != 2 ) error();
	if( fscanf(fin,"%s %s " , dummy, recursfile ) != 2 ) error();

	//printout parameters to check
	fprintf(stderr,"nsteps %ld\n" ,nsteps);
	fprintf(stderr,"print %ld\n"  ,print);
	fprintf(stderr,"mass %g\n"    ,mass);
	fprintf(stderr,"timestep %g\n",dt);
	fprintf(stderr,"kspring %g\n" ,kspring);
	fprintf(stderr,"d0 %g\n"      ,d0);
	fprintf(stderr,"inputxyz %s\n",inputxyz);
	fprintf(stderr,"trajfile %s\n",trajfile);
	fprintf(stderr,"trajfile %s\n",recursfile);

	fclose(fin);
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
	if( (x0=(double *)  calloc(N, sizeof(double))) == NULL ) n++;
	if( (yy0=(double *)  calloc(N, sizeof(double))) == NULL ) n++;
	if( (z0=(double *)  calloc(N, sizeof(double))) == NULL ) n++;
	if( (xprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (yprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (zprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (xtmp=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (ytmp=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (ztmp=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vx=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vy=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vz=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vx0=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vy0=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vz0=(double *) calloc(N, sizeof(double))) == NULL ) n++;
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

	//save reference coordinates for recursion
	for(i=0;i<N;i++){ x0[i]=x[i]; yy0[i]=y[i]; z0[i]=z[i]; Vx0[i]=Vx[i]; Vy0[i]=Vy[i]; Vz0[i]=Vz[i];  }
}

double dist(int i, int j){
	return sqrt( square(x[i]-x[j]) + square(y[i]-y[j]) + square(z[i]-z[j]) );
}

double dist0(int i, int j){
	return sqrt( square(x0[i]-x0[j]) + square(yy0[i]-yy0[j]) + square(z0[i]-z0[j]) );
}

void calc_forces(){
	int i; double effe;
	//reset the forces
	for (i=0;i<N;i++) { Fx[i]=Fy[i]=Fz[i]=0; }

	//sum up forces
	for (i=0;i<N-1;i++) {
		effe = -kspring*(dist(i,i+1) - d0)/dist(i,i+1);
		Fx[i] += effe*(x[i] - x[i+1]); Fx[i+1] += -effe*(x[i] - x[i+1]);
		Fy[i] += effe*(y[i] - y[i+1]); Fy[i+1] += -effe*(y[i] - y[i+1]);
		Fz[i] += effe*(z[i] - z[i+1]); Fz[i+1] += -effe*(z[i] - z[i+1]);
	}
}

void calc_energies(){
	int i;

	ekin=0;
	for(i=0;i<N;i++)
	    ekin += 0.5 * mass * ( Vx[i]*Vx[i] + Vy[i]*Vy[i] + Vz[i]*Vz[i] );

	epot=0;
	for (i=0;i<N-1;i++)
	    epot += 0.5*kspring*square( dist(i,i+1) - d0 );

	etot=ekin+epot;
}

void Euler_integrator(){
	int i;

	calc_forces(); // update the forces

	//update positions
	for (i=0;i<N;i++){
		x[i] += Vx[i]*dt;
		y[i] += Vy[i]*dt;
		z[i] += Vz[i]*dt;
	}

	// update velocities
	for (i=0;i<N;i++){
		Vx[i] += Fx[i] / mass * dt;
		Vy[i] += Fy[i] / mass * dt;
		Vz[i] += Fz[i] / mass * dt;
	}
}

void Verlet_integrator(){
	int i;
	static int once=1;

	calc_forces(); // update the forces

	//running Euler at first step
	if(once){
	    //saving old coords
	    for (i=0;i<N;i++){ xprev[i]=x[i]; yprev[i]=y[i]; zprev[i]=z[i]; }
	    Euler_integrator();
	    once=0;
	    return;
	}

	//storing coords in temporary arrays
	for (i=0;i<N;i++){ xtmp[i]=x[i]; ytmp[i]=y[i]; ztmp[i]=z[i]; }

	//update positions
	for (i=0;i<N;i++){
		x[i] = 2.*x[i] - xprev[i] + Fx[i]*dt*dt/mass;
		y[i] = 2.*y[i] - yprev[i] + Fy[i]*dt*dt/mass;
		z[i] = 2.*z[i] - zprev[i] + Fz[i]*dt*dt/mass;
	}

	//update velocities
	for (i=0;i<N;i++){
		Vx[i] = ( x[i] - xprev[i] )/(2. * dt);
		Vy[i] = ( y[i] - yprev[i] )/(2. * dt);
		Vz[i] = ( z[i] - zprev[i] )/(2. * dt);
	}

	//saving previous coords
	for (i=0;i<N;i++){ xprev[i]=xtmp[i]; yprev[i]=ytmp[i]; zprev[i]=ztmp[i]; }
}

void velocity_Verlet_integrator(){
	int i; static int once=1;
	if(once){ once=0; calc_forces(); }

	// update velocities(1)
	for (i=0;i<N;i++){
		Vx[i] += dt*Fx[i]/(2*mass);
		Vy[i] += dt*Fy[i]/(2*mass);
		Vz[i] += dt*Fz[i]/(2*mass);
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
		Vx[i] += dt*Fx[i]/(2*mass);
		Vy[i] += dt*Fy[i]/(2*mass);
		Vz[i] += dt*Fz[i]/(2*mass);
	}
}

void write_traj(){
	static int once=1;
	int i;
	if(once){
		once=0;
		if((ftraj=fopen(trajfile,"w"))==NULL) { fprintf(stderr,"ERROR while saving trajectory file\n"); exit(1); }
	}

	fprintf(ftraj,"%d\nTIME: %g\n",N,dt*nstep);
	for(i=0;i<N;i++){
		fprintf(ftraj,"%c\t%.6f\t%.6f\t%.6f\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%6e\n" ,atomtype[i],x[i],y[i],z[i],Vx[i],Vy[i],Vz[i],Fx[i],Fy[i],Fz[i]);
	}
	fflush(ftraj);
}

void printout(){
	static int once=1;
	if(once){
		once=0;
		printf("#time\tekin\t\tepot\t\tetot\t\tchain_length\n");
	}
	calc_energies();
	printf("%g\t%lf\t%lf\t%lf\t%lf\n",dt*nstep,ekin,epot,etot,dist(0,N-1) );
	write_traj();
}

/****************************************************
 Functions to calculate recursion times
****************************************************/
double Dv(){
  int i; double res=0;

  // difference in momenta
  for (i=0;i<N;i++)
    res += square(Vx[i]-Vx0[i])+square(Vy[i]-Vy0[i])+square(Vz[i]-Vz0[i]);

  return sqrt(res);
}

double Dd(){
  int i; double res=0;

  // difference in positions
  for (i=0;i<N-1;i++)
     res += square( dist(i,i+1) - dist0(i,i+1) );

  return sqrt(res);
}

void recursion(){
  static int once=1,exited=0;
  double DD=0.6;
  double DV=4.5;
  double dv,dd;

  if(once){
      once=0;
      if((frec=fopen(recursfile,"w"))==NULL) { fprintf(stderr,"ERROR while saving recursion file\n"); exit(1); }
  }

  dv = Dv();
  dd = Dd();

  // check if recurrent
  if ( exited && dd < DD && dv < DV ) // if trajectory returns into the box
      {
        fprintf(frec,"%lf\n",dt*nstep); fflush(frec);	// print time
	exited=0;
      }
  else if ( dd > DD || dv > DV ) exited = 1;  // if exits the box

  return;
}


int main(int argc, char *argv[])
{
   // read parameter file
   if (argc != 2) {fprintf(stderr,"Provide parameter file\n"); exit(1);}
   read_parameters(argv[1]);

   // read initial conditions
   readxyz();

   // write trajectory and print some information
   printout();

   // dynamics loop
   for (nstep=1;nstep<=nsteps;nstep++)
   {

      velocity_Verlet_integrator(); // one time of dynamics

	  recursion();
      // write trajectory and print some information
      if ( nstep % print == 0 ) printout();
   }

   fclose(ftraj);
   fclose(frec);

   return 0;
}


