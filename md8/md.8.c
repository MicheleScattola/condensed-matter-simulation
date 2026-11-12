#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>           /*for gettimeofday()   */
#include <fenv.h>               /*for feenableexcept() */

#define twopi 6.28318530718
#define kB 0.08617718  /*  kB= 1./11.604=0.086 meV/K  */
/* UNITS: meV, ps, Ang -- mass is in units of ~1.602e-26 kg ~= 9.648 amu  */

int N;             //number of atoms
char *atomtype;    //atomic species (1 char)
double dt,halfdt;  //timestep and half of it
double mass;       //mass
double kspring;    //spring constant
double d0;         //equilibrium spring distance
double epsilon,four_epsilon,twenty4_epsilon; //LJ interaction: epsilon, 4*epsilon, 24*epsilon
double sigma,sigma6,sigma12,two_sigma12; //LJ interaction: sigma, sigma^6, sigma^12, 2*sigma^12
double LJ_cutoff2; // square cutoff for LJ interaction
long int nsteps;   //number of simulation steps
long int nstep;    //actual step, changing during simulation
long int print;    //print every these steps
double *x,*y,*z,*Vx,*Vy,*Vz,*Fx,*Fy,*Fz;
double *xprev,*yprev,*zprev,*xtmp,*ytmp,*ztmp; // to store the old coordinates for Verlet
double epot,ekin,etot,Tm,Tin;
double gam; // gam in reciprocal time units, ps^-1  (F = -m*v*gam)
char inputxyz[30],trajfile[30];
FILE *ftraj;
double Fmax=0;

//'static' forces the linker to include the function symbol in the symbol table
static inline double square(double val){ return (val*val);     }
static inline double cube(double val)  { return (val*val*val); }
static inline double distx(int i,int j){ return (x[i]-x[j]);   }
static inline double disty(int i,int j){ return (y[i]-y[j]);   }
static inline double distz(int i,int j){ return (z[i]-z[j]);   }
static inline double dist2(int i,int j){ return ( square(distx(i,j)) + square(disty(i,j)) + square(distz(i,j)) ); }
static inline double dist(int i, int j){ return sqrt( dist2(i,j) );    }
static inline double absd(double val)  { return (val<0. ? -val : val); }

void read_parameters(char *filename){
	char dummy[100];
	FILE *fin;
	void error() { fprintf(stderr,"ERROR in input parameters file %s\n",filename); exit(1); }

	if((fin=fopen(filename,"r"))==NULL)                error();
	if( fscanf(fin,"%s %ld ", dummy, &nsteps  ) != 2 ) error();
	if( fscanf(fin,"%s %ld ", dummy, &print   ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &mass    ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &dt      ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &kspring ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &d0      ) != 2 ) error();
	if( fscanf(fin,"%s %s  ", dummy, inputxyz ) != 2 ) error();
	if( fscanf(fin,"%s %s  ", dummy, trajfile ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &epsilon ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &sigma   ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &Tin     ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &gam     ) != 2 ) error();

	//printout parameters to check
	fprintf(stderr,"nsteps %ld\n"    ,nsteps);
	fprintf(stderr,"print %ld\n"     ,print);
	fprintf(stderr,"mass %g\n"       ,mass);
	fprintf(stderr,"timestep %g\n"   ,dt);
	fprintf(stderr,"kspring %g\n"    ,kspring);
	fprintf(stderr,"restlength %g\n" ,d0);
	fprintf(stderr,"inputxyz %s\n"   ,inputxyz);
	fprintf(stderr,"trajfile %s\n"   ,trajfile);
	fprintf(stderr,"LJ_epsilon %g\n" ,epsilon);
	fprintf(stderr,"LJ_sigma %g\n"   ,sigma);
	fprintf(stderr,"temperature %g\n",Tin);
	fprintf(stderr,"gamma %g\n"      ,gam);

	fclose(fin);
	halfdt=dt/2.;
	four_epsilon=4.*epsilon;
	twenty4_epsilon=24.*epsilon;
	sigma6=pow(sigma,6);
	sigma12=sigma6*sigma6;
	two_sigma12=2.*sigma12;
	LJ_cutoff2=square(3.*sigma);
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

void calc_forces(){
	int i,j; double effe,d2,d8,dX,dY,dZ;
	//reset the forces
	for (i=0;i<N;i++) { Fx[i]=Fy[i]=Fz[i]=0; }

	//chain
	for (i=0;i<N-1;i++){
		    effe = -kspring*(dist(i,i+1) - d0)/dist(i,i+1);
		    Fx[i] += effe*distx(i,i+1); Fx[i+1] += -effe*distx(i,i+1);
		    Fy[i] += effe*disty(i,i+1); Fy[i+1] += -effe*disty(i,i+1);
		    Fz[i] += effe*distz(i,i+1); Fz[i+1] += -effe*distz(i,i+1);
	}
	//LJ
	for (i=0;i<N;i++)
	    for (j=0;j<i;j++){
		d2=dist2(i,j);
		if(d2>LJ_cutoff2) continue;
		d8=d2*d2*d2*d2;
		effe=twenty4_epsilon*(two_sigma12*d2/(d8*d8)-sigma6/d8);
		dX=distx(i,j); dY=disty(i,j); dZ=distz(i,j);
		Fx[i]+=effe*dX; Fx[j]-=effe*dX;
		Fy[i]+=effe*dY; Fy[j]-=effe*dY;
		Fz[i]+=effe*dZ; Fz[j]-=effe*dZ;
	}
}

void calc_energies(){
	int i,j; double d2,d6;

	ekin=0;
	for(i=0;i<N;i++){ ekin+=( Vx[i]*Vx[i] + Vy[i]*Vy[i] + Vz[i]*Vz[i] ); }

	ekin *= 0.5*mass;
	Tm = 2*ekin/(3.*N*kB);

	epot=0;
	//chain
	for (i=0;i<N-1;i++)
		epot += 0.5*kspring*square( dist(i,i+1) - d0 );

	//LJ
	for (i=0;i<N;i++)
	    for (j=0;j<i;j++){
		d2=dist2(i,j);
		if(d2>LJ_cutoff2) continue;
		d6=d2*d2*d2;
		epot+=four_epsilon*(sigma12/(d6*d6)-sigma6/d6);
        }

	etot=ekin+epot;
}


double Fgauss(){ //gaussian-distributed random force
	static int once=1,i=0;
	static double gen1,gen2,var;
	if(once){
		once=0;
		srand(4567);
		var=2.*gam*kB*Tin/halfdt; //velocity advances in steps of halfdt
	}
	if(Tin==0.) return 0.0;
	if(i==0){
		gen1=(rand()+1.)/(RAND_MAX+1.);
		gen2=(rand()+1.)/(RAND_MAX+1.);
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

	fprintf(ftraj,"%d\nTIME: %g\n",N,dt*nstep);
	for(i=0;i<N;i++){
		fprintf(ftraj,"%c\t%.6f\t%.6f\t%.6f\t%.6e\t%.6e\t%.6e\t%.6e\t%.6e\t%6e\n" ,atomtype[i],x[i],y[i],z[i],Vx[i],Vy[i],Vz[i],Fx[i],Fy[i],Fz[i]);
	}
	fflush(ftraj);
}

double gyration(){
	int i; double xcm=0,ycm=0,zcm=0,res=0;
	for(i=0;i<N;i++){ xcm+=x[i]; ycm+=y[i]; zcm+=z[i]; }
	xcm/=N; ycm/=N; zcm/=N;
	for(i=0;i<N;i++)
	    res+=( square(x[i]-xcm) + square(y[i]-ycm) + square(z[i]-zcm) );

	return sqrt(res/N);
}

void printout(){
	static int once=1;
	if(once){
		once=0;
		printf("#time\tekin\t\tepot\t\tetot\t\ttemperature\tchain_length\tgyration\n");
	}
	calc_energies();
	printf("%g\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dt*nstep,ekin,epot,etot,Tm,dist(0,N-1),gyration() );
	write_traj();
	fflush(stdout);
}

int main(int argc, char *argv[])
{
    feenableexcept(FE_DIVBYZERO|FE_INVALID|FE_OVERFLOW);
    struct timeval sec0,sec1,sec2; int secM, progress_time=2; //timer
    gettimeofday(&sec0,NULL); sec1=sec0;
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

	if(nstep%progress_time==1){
	    gettimeofday(&sec2,NULL);
	    secM=round( sec2.tv_sec-sec1.tv_sec + (sec2.tv_usec-sec1.tv_usec)/1e6 ) * (nsteps-nstep)/progress_time;
	    sec1=sec2;
	    if(secM>0){ //secM is zero if sec2-sec1 is less than half second
		fprintf(stderr,"\rprogress: %.01lf%%   remaining time: %02d:%02d:%02d ",100.*nstep/nsteps,secM/3600,(secM%3600)/60,secM%60);
		fflush(stderr);
	    }
	    else progress_time*=2;
	}

	vVerlet_Langevin(); // one time of dynamics
	// write trajectory and print some information
	if ( nstep % print == 0 ) printout();
    }

    fclose(ftraj);
    gettimeofday(&sec2,NULL); secM=round(sec2.tv_sec-sec0.tv_sec + (sec2.tv_usec-sec0.tv_usec)/1e6);
    fprintf(stderr,"\relapsed:  %02d:%02d:%02d                             \n",secM/3600,(secM%3600)/60,secM%60);
    fflush(stderr);

    return 0;
}


