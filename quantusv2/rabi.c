// rabi.c - CORRECTED VERSION
#include "rabi.h"
#include <math.h>
#include <stdlib.h>
#define M_PI 3.14159265358979323846

void rabi_init(rabidrive *drive, double max_rabi_freq, double dt){
    drive->brightness = 0.0;
    drive->max_rabi_freq = max_rabi_freq;
    drive->omega = 0.0;
    drive->dt = dt;
    drive->t = 0.0;
    drive->detuning = 0.1 * ((double)rand()/RAND_MAX - 0.5); // Small random detuning.
}

void rabi_set_brightness(rabidrive *drive, double b){
    drive->brightness = b;
    // Scale the hamiltonian operator by brightness
    drive->omega = 2.0 * M_PI * drive->max_rabi_freq * b; 
}

double rabi_get_omega(const rabidrive *drive){
    return drive->omega;
}

void rabi_step(rabidrive* drive, pqubit pq){
    if(pq->measured != -1){ //if already collapsed, do nothing.
        return;
    }
    
   double Omega = drive->omega;      //changes every call based on brightness.
    double Delta = drive->detuning;   //constant
    
    // Compute propagator for the static Hamiltonian
    double R = sqrt(Omega*Omega + Delta*Delta);
    double angle = R * drive->dt / 2.0;
    double c = cos(angle);
    double s = sin(angle);
     //safeguard, in case R is zero.
    double nx = 0; 
    double nz = 0; 
    if(R < 1e-15) {
        nx = 0.0;
        nz = 0.0;
    }
    else{
        nx = Omega / R;
        nz = Delta / R;
    } 
    
    
    // Apply the propagator to the qubit state
    double complex a0 = pq->alpha, b0 = pq->beta;
    pq->alpha = (c - I*nz*s)*a0 - I*nx*s*b0;
    pq->beta  = -I*nx*s*a0 + (c + I*nz*s)*b0;
    
    drive->t += drive->dt;  // move to next timestep
}