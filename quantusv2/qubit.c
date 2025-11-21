

#include "qubit.h"
#include <stdlib.h>
#include<math.h>
void qubit_init(pqubit pq, int index){
    pq->alpha = 0.8 + 0.3 * I;
    pq ->beta = 0.6 - 0.2 * I;
    pq-> measured = -1;
    pq->partner = -1;
    pq->index = index;
    double norm = sqrt(cabs(pq->alpha)*cabs(pq->alpha) + cabs(pq->beta)*cabs(pq->beta));
    pq->alpha /= norm;
    pq->beta /= norm;
}

void qubit_measure(pqubit pq){
    if(pq->measured != -1){
        return;
    }
    double p0 = cabs(pq->alpha);
    p0 *= p0; //computes the probability of measuring 0. 

    int result = (rand() / (double)RAND_MAX < p0) ? 0:1; //if random number/max possible random number <p0, then pq->measured = 0. else, pq->measured = 1.

    if(result == 0){
        pq->alpha = 1.0 + 0.0 * I;
        pq ->beta = 0.0 + 0.0 * I;
    }
    else{
        pq->alpha = 0.0 + 0.0 * I;
        pq->beta = 1.0 + 0.0 * I;
    }
    pq->measured = result;

}
int qubit_is_collapsed(const pqubit pq){
    return pq->measured;
}