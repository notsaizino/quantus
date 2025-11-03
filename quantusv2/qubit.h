#ifndef QUBIT_H
#define QUBIT_H

#include <complex.h>

typedef struct {
    double complex alpha;
    double complex beta;
    int index; //identifies which qubit it is.
    int measured; //current state. -1 = not measured; 0 = |0>, 1 = |1>;
    int partner; //int value to know if there is entanglement or not.
} qubit, *pqubit;

void qubit_init(pqubit pq, int index);
void qubit_measure(pqubit pq);
int qubit_is_collapsed(const pqubit pq);  // FIXED: Match implementation name

#endif