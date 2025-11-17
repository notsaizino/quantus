// |ψ(t)⟩ = cos(Ωt/2) |0⟩ − i sin(Ωt/2) |1⟩, with Ω => rabi oscillator. 

#ifndef RABI_H
#define RABI_H

#include "qubit.h"

typedef struct _rabidrive{
    double brightness; //computed by an AI
    double max_rabi_freq; // maximum frequency possible. 
    double omega; //current rabi oscillator
    double t; //elapsed time
    double dt; //timestep.
    double detuning;
}rabidrive, *prabidrive;

void rabi_init(rabidrive *drive, double max_freq, double dt); //initiates the rabi function.
void rabi_set_brightness(rabidrive *drive, double b); // sets the brightness which i use to calculate rabi oscillator.
void rabi_step(rabidrive* drive, pqubit pq); //updates qubit rabi oscillator.
double rabi_get_omega(const rabidrive *drive);

#endif