#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define twopi 6.28318530718
#define kB 0.08617718  /*  kB= 1./11.604=0.086 meV/K  */
/* UNITS: meV, ps, Ang -- mass is in units of ~1.602e-26 kg ~= 9.648 amu  */

int N;             //number of atoms
char *atomtype;    //atomic species (1 char)
double mass;       //mass
double kspring;    //spring constant
double d0;         //equilibrium spring distance
double side,halfside;//side of the box
long int nsteps;   //number of simulation steps
long int nstep;    //actual step, changing during simulation
long int print;    //print every these steps
double *x,*y,*z,*Vx,*Vy,*Vz;
double *xprev,*yprev,*zprev,*Vxprev,*Vyprev,*Vzprev; // to store the old coordinates for Verlet
double epot,ekin,etot,Tm,Tin;
double gam; // factor for MC move 1 = kB*T
char inputxyz[30],trajfile[30];
FILE *ftraj;
double vmax,halfvmax,xmax,halfxmax;
int trials;
unsigned int random_seed = 4567;

double square(double val){ return val*val; }
double cube(double val){ return val*val*val; }

void read_parameters(char *filename){
	char dummy[300];
	FILE *fin;
	void error() { fprintf(stderr,"ERROR in input parameters file %s\n",filename); exit(1); }

	if((fin=fopen(filename,"r"))==NULL) error();
	if( fscanf(fin,"%s %ld ", dummy, &nsteps  ) != 2 ) error();
	if( fscanf(fin,"%s %ld ", dummy, &print   ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &mass    ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &kspring ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &d0      ) != 2 ) error();
	if( fscanf(fin,"%s %s  ", dummy, inputxyz ) != 2 ) error();
	if( fscanf(fin,"%s %s  ", dummy, trajfile ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &side    ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &Tin     ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &gam     ) != 2 ) error();

	//printout parameters to check
	fprintf(stderr,"nsteps %ld\n"    ,nsteps);
	fprintf(stderr,"print %ld\n"     ,print);
	fprintf(stderr,"mass %g\n"       ,mass);
	fprintf(stderr,"kspring %g\n"    ,kspring);
	fprintf(stderr,"d0 %g\n"         ,d0);
	fprintf(stderr,"inputxyz %s\n"   ,inputxyz);
	fprintf(stderr,"trajfile %s\n"   ,trajfile);
	fprintf(stderr,"side %g\n"       ,side);
	fprintf(stderr,"temperature %g\n",Tin);
	fprintf(stderr,"gam %g\n"        ,gam);

	fclose(fin);
	halfside=side/2.;
	vmax=gam*sqrt(kB*Tin/mass);
	halfvmax=0.5*vmax;
	xmax=gam*sqrt(2*kB*Tin/kspring);
	halfxmax=0.5*xmax;

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
	if( (Vxprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vyprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vzprev=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vx=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vy=(double *) calloc(N, sizeof(double))) == NULL ) n++;
	if( (Vz=(double *) calloc(N, sizeof(double))) == NULL ) n++;
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

void gen_trial(){
	for(int i=0;i<N;i++){
	    x[i] = xprev[i] + 2.*xmax*rand()/RAND_MAX - xmax; // -xmax..xmax
	    y[i] = yprev[i] + 2.*xmax*rand()/RAND_MAX - xmax; // -xmax..xmax
	    z[i] = zprev[i] + 2.*xmax*rand()/RAND_MAX - xmax; // -xmax..xmax
	    Vx[i] = Vxprev[i] + 2.*vmax*rand()/RAND_MAX - vmax; // -vmax..vmax
	    Vy[i] = Vyprev[i] + 2.*vmax*rand()/RAND_MAX - vmax; // -vmax..vmax
	    Vz[i] = Vzprev[i] + 2.*vmax*rand()/RAND_MAX - vmax; // -vmax..vmax
	}
}

void MC_move(){
	int i; static int once=1;
	double E0,E1,deltaE,r;
	if(once){ once=0; srand(random_seed); }

	for(i=0;i<N;i++){ xprev[i]=x[i]; yprev[i]=y[i]; zprev[i]=z[i]; Vxprev[i]=Vx[i]; Vyprev[i]=Vy[i]; Vzprev[i]=Vz[i]; }

	calc_energies();E0=etot;

	trials=0;
	while(1){
	    trials++;
	    gen_trial();
	    calc_energies();E1=etot;
	    deltaE=E1-E0;
	    if(deltaE<0.) return; //accept
	    r=1.*rand()/RAND_MAX;
	    if( r < exp(-deltaE/(kB*Tin)) ) return; //accept
	}
}

void write_traj(){
	static int once=1;
	int i;
	if(once){
		once=0;
		if((ftraj=fopen(trajfile,"w"))==NULL) { fprintf(stderr,"ERROR while saving trajectory file\n"); exit(1); }
	}

	fprintf(ftraj,"%d\nLattice=\"%g 0.0 0.0  0.0 %g 0.0  0.0 0.0 %g\"\tMC STEP: %ld\n",N,side,side,side,nstep);
	for(i=0;i<N;i++){
		fprintf(ftraj,"%c\t%.6f\t%.6f\t%.6f\t%.6e\t%.6e\t%.6e\n" ,atomtype[i],x[i],y[i],z[i],Vx[i],Vy[i],Vz[i]);
	}
	fflush(ftraj);
}

void printout(){
	static int once=1;
	if(once){
		once=0;
		printf("#time\tekin\t\tepot\t\tetot\t\tchain_length\ttemperature\ttrials\n");
	}
	calc_energies();
	printf("%ld\t%lf\t%lf\t%lf\t%lf\t%lf\t%d\n",nstep,ekin,epot,etot,dist(0,N-1),Tm,trials );
	write_traj();
	fflush(stdout);
}

int temperature_window_reached(){
	static double temperatures[5];
	double average=0.;
	int i;

	temperatures[(nstep-1)%5]=Tm;
	if(nstep<5) return 0;
	for(i=0;i<5;i++) average+=temperatures[i];
	return fabs(average/5.-100.)<=1.;
}


int main(int argc, char *argv[])
{
   int reached=0;
   // read parameter file
   if (argc < 2) {fprintf(stderr,"Provide parameter file\n"); exit(1);}
   read_parameters(argv[1]);
	if (argc > 2) {
		gam = atof(argv[2]);
		vmax = gam*sqrt(kB*Tin/mass);
		halfvmax = 0.5*vmax;
		xmax = gam*sqrt(2*kB*Tin/kspring);
		halfxmax = 0.5*xmax;
		fprintf(stderr,"Using custom gamma = %g\n",gam);
	}
	if (argc > 3) random_seed = (unsigned int)strtoul(argv[3], NULL, 10);


   // read initial conditions
   readxyz();

   // write trajectory and print some information
   printout();

   // dynamics loop
   for (nstep=1;nstep<=nsteps;nstep++)
   {

      MC_move(); // one accepted MC step

	if (temperature_window_reached()) {
	 printout();
	 reached=1;
	 break;
	}
	if ( nstep % print == 0 ) printout();
   }

   fclose(ftraj);

	return reached ? 0 : 1;
}


