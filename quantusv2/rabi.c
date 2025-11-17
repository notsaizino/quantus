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
    // Scale the frequency more aggressively to see visible changes
    drive->omega = 2.0 * M_PI * drive->max_rabi_freq * b; // Using b^2 for more dynamic range
}

double rabi_get_omega(const rabidrive *drive){
    return drive->omega;
}

void rabi_step(rabidrive* drive, pqubit pq){
    if(pq->measured != -1){ //if already collapsed, do nothing.
        return;
    }
    
    // Get current state components
    double complex a0 = pq->alpha;
    double complex b0 = pq->beta;
    
    // Calculate rotation angle based on current omega and timestep
    double theta = drive->omega * drive->dt;
    double thetadet = drive->detuning * drive->dt;  // detuning effect
    
    //apply the rotation matrixes.
    // X-axis Rabi rotation, following Pauli matrice:
    // [ cos(θ/2)   -i·sin(θ/2) ]
    // [ -i·sin(θ/2)   cos(θ/2) ]
    double complex tempalphax = cos(theta /2.0)*a0 - I * sin(theta/2.0)*b0;
    double complex tempbetax  = -I * sin(theta/2.0)*a0 + cos(theta /2.0)*b0;
    // Y-axis Rabi rotation, following Pauli matrice:
    //[cos(θ/2)    sin(θ/2)] * [tempalpha]
    //[​−sin(θ/2)   cos(θ/2)​]   [tempbeta]
    
    double complex tempalphay = tempalphax* cos(thetadet /2.0)- tempbetax*sin(thetadet/2.0);
    double complex tempbetay = tempalphax * sin(thetadet/2.0) + tempbetax*cos(thetadet /2.0);
    //Z-axis Rabi rotation, following Pauli matrice:
    //[e^(−iθ/2)     0     ] * [tempalpha]
    //[   0      e^(iθ/2) ]     [tempbeta]

    pq->alpha = tempalphay * (cos(theta /2.0) - I*sin(theta/2.0));
    pq->beta = tempbetay * (cos(theta /2.0) + I*sin(theta/2.0));

    // Ensure normalization (important for numerical stability.)
    double norm = sqrt(cabs(pq->alpha) * cabs(pq->alpha) + cabs(pq->beta) * cabs(pq->beta));
    if (norm > 0) {
        pq->alpha /= norm;
        pq->beta /= norm;
    }
    
    drive->t += drive->dt;
}