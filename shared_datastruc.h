#ifndef SHARED_DATASTRUC_H
#define SHARED_DATASTRUC_H

#include <pthread.h>
#include <semaphore.h>
#include <string>
#include "car_assemble.h"  
// car_assemble.h provides:
//   enum PowertrainType { GasEngine, HybridEngine, PowertrainTypeN };
//   enum ChassisRobotType { Titano, MegaForce, ChassisRobotTypeN };

struct SharedData {
    // ----- Powertrain Queue (capacity = 20) -----
    static const int PT_QUEUE_SIZE = 20;
    PowertrainType pt_queue[PT_QUEUE_SIZE];
    int pt_front;
    int pt_rear;
    int pt_count;
    
    // ----- Chassis Queue (capacity = 15) -----
    static const int CH_QUEUE_SIZE = 15;
    std::string ch_queue[CH_QUEUE_SIZE];
    int ch_front;
    int ch_rear;
    int ch_count;
    
    // ----- Production Counters -----
    int production_limit;          // -n: total number to produce
    int produced_total;            // Overall produced count
    int powertrain_consumed_total; // Overall count of powertrains consumed
    int chassis_consumed;          // Overall chassis consumed (by RoboMount)
    
    // ----- Logging Counters -----
    unsigned int produced[PowertrainTypeN];        // Produced count per type
    unsigned int inAssemblyQueue[PowertrainTypeN];   // Count waiting in queue per type
    unsigned int consumed[ChassisRobotTypeN][PowertrainTypeN]; // Count consumed by each consumer per type
    
    // ----- Synchronization for Powertrain Queue -----
    sem_t pt_empty;  // Initially PT_QUEUE_SIZE
    sem_t pt_full;   // Initially 0
    pthread_mutex_t pt_mutex;
    
    // ----- Type Limit Semaphores -----
    // Even though the queue has a fixed capacity, these semaphores simulate
    // the limit on concurrently held items of each type.
    sem_t gas_sem;     // Initially 12
    sem_t hybrid_sem;  // Initially 8
    
    // ----- Synchronization for Chassis Queue -----
    sem_t ch_empty;  // Initially CH_QUEUE_SIZE
    sem_t ch_full;   // Initially 0
    pthread_mutex_t ch_mutex;
};

extern SharedData shared;

void init_shared_data(int production_limit);
void destroy_shared_data();

#endif // SHARED_DATASTRUC_H
