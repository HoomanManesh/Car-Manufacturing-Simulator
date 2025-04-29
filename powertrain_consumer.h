#ifndef POWERTRAIN_CONSUMER_H
#define POWERTRAIN_CONSUMER_H

#include "car_assemble.h"  // Provides ChassisRobotType enum

// Entry function for powertrain consumer threads.
// The argument is a pointer to a ChassisRobotType variable (Titano or MegaForce).
void* powertrain_consumer(void* arg);

#endif // POWERTRAIN_CONSUMER_H
