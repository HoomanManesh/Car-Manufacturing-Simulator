#ifndef POWERTRAIN_PRODUCER_H
#define POWERTRAIN_PRODUCER_H

#include "car_assemble.h"  // Provides PowertrainType enum

// Entry function for powertrain producer threads.
// The argument is a pointer to a PowertrainType variable (GasEngine or HybridEngine).
void* powertrain_producer(void* arg);

#endif // POWERTRAIN_PRODUCER_H
