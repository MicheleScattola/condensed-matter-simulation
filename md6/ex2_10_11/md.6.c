#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define twopi 6.28318530718
#define kB 0.08617718  /*  kB= 1./11.604=0.086 meV/K  */
/* UNITS: meV, ps, Ang -- mass is in units of ~1.602e-26 kg ~= 9.648 amu  */

long int nsteps;   //number of simulation steps
long int nstep;    //actual step, changing during simulation
long int print;    //print every these steps
double mass;       //mass
double dt,halfdt;  //timestep and half of it
double kspring;    //spring constant
double d0;         //equilibrium spring distance
char inputxyz[30]; //initialfile
char trajfile[30]; //trajfile
FILE *ftraj;

int N;             //number of atoms
char *atomtype;    //atomic species (1 char)
double *x,*y,*z,*Vx,*Vy,*Vz,*Fx,*Fy,*Fz;
double epot,ekin,etot,Tm,Tin;
double gam; // gam in reciprocal time units, ps^-1  (F = -m*v*gam)
double side,halfside;//side of the box

double c6,c10,d0go,cutoff,cutoff2; // GO: potential parameters
int dmin;                          // GO: number of neighbors to skip
int **map;                         // GO: contact map Mij
int ncontacts,ncontacts0;          // GO: number of contacts in native Mij
double *xN,*yN,*zN;                // GO: native coords
char nativexyz[20];                // GO: native input file

double square(double val){ return val*val; }
double cube(double val){ return val*val*val; }
double distx(int i, int j){ return x[i]-x[j]; }
double disty(int i, int j){ return y[i]-y[j]; }
double distz(int i, int j){ return z[i]-z[j]; }
double dist (int i, int j){ return sqrt( square(x[i]-x[j])   + square(y[i]-y[j])   + square(z[i]-z[j]) ); }
double dist2(int i, int j){ return     ( square(x[i]-x[j])   + square(y[i]-y[j])   + square(z[i]-z[j]) ); }
double distN(int i, int j){ return sqrt( square(xN[i]-xN[j]) + square(yN[i]-yN[j]) + square(zN[i]-zN[j]) ); }

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
	if( fscanf(fin,"%s %lf ", dummy, &Tin     ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &gam     ) != 2 ) error();
	if( fscanf(fin,"%s %s  ", dummy, nativexyz) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &c6      ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &c10     ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &d0go    ) != 2 ) error();
	if( fscanf(fin,"%s %d  ", dummy, &dmin    ) != 2 ) error();
	if( fscanf(fin,"%s %lf ", dummy, &cutoff  ) != 2 ) error();

	//printout parameters to check
	fprintf(stderr,"nsteps %ld\n" ,nsteps);
	fprintf(stderr,"print %ld\n"  ,print);
	fprintf(stderr,"mass %g\n"    ,mass);
	fprintf(stderr,"timestep %g\n",dt);
	fprintf(stderr,"kspring %g\n" ,kspring);
	fprintf(stderr,"restlength %g\n"      ,d0);
	fprintf(stderr,"inputxyz %s\n",inputxyz);
	fprintf(stderr,"trajfile %s\n",trajfile);
	fprintf(stderr,"temperature %g\n",Tin);
	fprintf(stderr,"gamma %g\n"   ,gam);
	fprintf(stderr,"nativefile %s\n",nativexyz);
	fprintf(stderr,"c6 %g\n"        ,c6);
	fprintf(stderr,"c10 %g\n"       ,c10);
	fprintf(stderr,"d0map %g\n"     ,d0go);
	fprintf(stderr,"dmin %d\n"      ,dmin);
	fprintf(stderr,"cutoff %g\n"    ,cutoff);

	fclose(fin);
	halfside=side/2.;
	halfdt=dt/2.;
	cutoff2=square(cutoff);
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

void ContactMap(){ // calculate contact map Mij, once at the beginning of the simulation
    int i,j,NN,n=0;
    double d2,d0go2=d0go*d0go;
    char dummy[300],dum2[10]; FILE *fin;
    void error(){ fprintf(stderr,"ERROR in native structure file\n"); exit(1); }
    if((fin=fopen(nativexyz,"r"))==NULL) error();
    fgets(dummy,sizeof(dummy),fin);
    if( sscanf(dummy,"%d",&NN) != 1 || NN != N ) error();
    if( (xN=(double *) calloc(N, sizeof(double))) == NULL ) n++;
    if( (yN=(double *) calloc(N, sizeof(double))) == NULL ) n++;
    if( (zN=(double *) calloc(N, sizeof(double))) == NULL ) n++;
    if( (map=(int **) malloc(N*sizeof(int *))) == NULL ) n++;
    for(i=0;i<N;i++) if( (map[i]=(int *) calloc(N,sizeof(int))) == NULL ) n++;
    if(n>0){fprintf(stderr,"Allocation error %d\n",n); exit(1);}
    fgets(dummy,sizeof(dummy),fin); //skip comment line
    for(i=0;i<N;i++){
	fgets(dummy,sizeof(dummy),fin);
	if( sscanf(dummy,"%s %lf %lf %lf",dum2,&xN[i],&yN[i],&zN[i]) !=4 ) error();
    }
    fclose(fin);

    ncontacts0=0;
    for (i=0;i<N;i++)		// loop on all atoms
      for (j=i+dmin;j<N;j++){  	// not for consecutive atoms
	map[i][j]=map[j][i]=0;
	d2=( square(xN[i]-xN[j]) + square(yN[i]-yN[j]) + square(zN[i]-zN[j]) );
	if (d2<d0go2){
	    map[i][j]=map[j][i]=1;
	    ncontacts0++;
	}
      }
    fprintf(stderr,"GO Contacts: %d\n",ncontacts0); fflush(stderr);
}

void Force_Go(){ // force of Go model
    int i,j;
    double effe,d2,d6,d8,d12;

    for (i=0;i<N;i++)		// loop on all atoms
      for (j=i+dmin;j<N;j++){	// not for consecutive atoms
	d2 = dist2(i,j);
	if ( d2 > cutoff2 ) continue;
	d6 = d2*d2*d2;
	d12 = d6*d6;

	//repulsive term
	effe = 10.*c10/d12;
	Fx[i] += effe*(x[i]-x[j]); Fx[j] += -effe*(x[i]-x[j]);
	Fy[i] += effe*(y[i]-y[j]); Fy[j] += -effe*(y[i]-y[j]);
	Fz[i] += effe*(z[i]-z[j]); Fz[j] += -effe*(z[i]-z[j]);

	//attractive term
	if ( map[i][j]==1 ){
	    d8 = d6*d2;
	    effe = -6.*c6/d8;
	    Fx[i] += effe*(x[i]-x[j]); Fx[j] += -effe*(x[i]-x[j]);
	    Fy[i] += effe*(y[i]-y[j]); Fy[j] += -effe*(y[i]-y[j]);
	    Fz[i] += effe*(z[i]-z[j]); Fz[j] += -effe*(z[i]-z[j]);
	}
    }
}

void Energy_Go(){ // force of Go model
    int i,j;
    double d2,d10,d6,d0go2=d0go*d0go;

    ncontacts=0;
    for (i=0;i<N;i++) 		// loop on all atoms
      for (j=i+dmin;j<N;j++){	// not for consecutive atoms
	d2 = dist2(i,j);
	if ( d2 > cutoff2 ) continue;

	d10 = d2*d2*d2*d2*d2;
	epot += c10/d10;
	if ( map[i][j]==1 ){
	    d6 = d2*d2*d2;
	    epot -= c6/d6;
	    if (d2<d0go2) ncontacts++;
	}
      }
}

double dRMSD(){ //order parameter
  int i,j; double val=0.;

   for (i=0;i<N;i++)
    for (j=i;j<N;j++){
          val += square(dist(i,j)-distN(i,j));
    }
   return sqrt( val*2./N/(N+1.) );
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

	//Go model
	Force_Go();
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

	Energy_Go();

	etot=ekin+epot;
}

double Fgauss(){ //gaussian-distributed random force
	static int once=1,i=0;
	static double gen1,gen2,var;
	if(once){
		once=0;
		srand( (int)(time(NULL)) );
		var=2*gam*kB*Tin/(dt/2); //velocity advances in steps of dt/2
	}
	if(Tin==0.) return 0.0;
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
		printf("#time\tekin\t\tepot\t\tetot\t\tchain_length\ttemperature\tq\t\tdRMSD\n");
	}
	calc_energies();
	printf("%g\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dt*nstep,ekin,epot,etot,dist(0,N-1),Tm,1.*ncontacts/ncontacts0,dRMSD());
	write_traj();
	fflush(stdout);
}


int main(int argc, char *argv[])
{
   // read parameter file
   if (argc != 2) {fprintf(stderr,"Provide parameter file\n"); exit(1);}
   read_parameters(argv[1]);

   // read initial conditions
   readxyz();

   // read native file and evaluate Mij
   ContactMap();

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


