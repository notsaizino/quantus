// rabi.c - CORRECTED VERSION
#include "rabi.h"
#include <math.h>
#define M_PI 3.14159265358979323846

void rabi_init(rabidrive *drive, double max_rabi_freq, double dt){
    drive->brightness = 0.0;
    drive->max_rabi_freq = max_rabi_freq;
    drive->omega = 0.0;
    drive->dt = dt;
    drive->t = 0.0;
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
    if(pq->measured != -1){
        return;
    }
    
    // Get current state components
    double complex a0 = pq->alpha;
    double complex b0 = pq->beta;
    
    // Calculate rotation angle based on current omega and timestep
    double theta = drive->omega * drive->dt;
    
    // Apply Rabi rotation matrix (pauli matrice):
    // [ cos(θ/2)   -i·sin(θ/2) ]
    // [ -i·sin(θ/2)   cos(θ/2) ]
    
    double cos_theta_2 = cos(theta / 2.0);
    double sin_theta_2 = sin(theta / 2.0);
    //apply the rotation matrix.
    // X-axis Rabi rotation, following Pauli matrice:
    // [ cos(θ/2)   -i·sin(θ/2) ]
    // [ -i·sin(θ/2)   cos(θ/2) ]
    double complex xrot = cos_theta_2*a0 - I * sin_theta_2*b0;
    double complex yrot  = -I * sin_theta_2*a0 + cos_theta_2*b0;
    // Y-axis Rabi rotation, following Pauli matrice:
    //[cos(θ/2)    sin(θ/2)] * [xrot]
    //[​−sin(θ/2)   cos(θ/2)​]   [yrot]
    //the above is the matrix multiplication representing a XY axis Rabi rotation.
    pq->alpha = xrot* cos_theta_2 - yrot*sin_theta_2;
    pq->beta = xrot * sin_theta_2 + yrot*cos_theta_2;
    // Ensure normalization (important for numerical stability.)
    double norm = sqrt(cabs(pq->alpha) * cabs(pq->alpha) + cabs(pq->beta) * cabs(pq->beta));
    if (norm > 0) {
        pq->alpha /= norm;
        pq->beta /= norm;
    }
    
    drive->t += drive->dt;
}