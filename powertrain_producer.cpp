#include "powertrain_producer.h"
#include "shared_datastruc.h"
#include "log.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// External production delays set in main.cpp.
extern int sleep_g;  // Delay for GasEngine production (ms)
extern int sleep_h;  // Delay for HybridEngine production (ms)

// Helper function to sleep (ms milliseconds)
static void msleep(int ms) {
    usleep(ms * 1000);
}

void* powertrain_producer(void* arg) {
    PowertrainType type = *(PowertrainType*)arg;
    
    while (true) {
        if (shared.produced_total >= shared.production_limit)
            break;
        
        // Simulate production delay.
        if (type == GasEngine)
            msleep(sleep_g);
        else
            msleep(sleep_h);
        
        // Wait on type-specific semaphore.
        if (type == GasEngine)
            sem_wait(&shared.gas_sem);
        else
            sem_wait(&shared.hybrid_sem);
        
        // Wait for an empty slot in the powertrain queue.
        sem_wait(&shared.pt_empty);
        pthread_mutex_lock(&shared.pt_mutex);
        
        if (shared.produced_total >= shared.production_limit) {
            pthread_mutex_unlock(&shared.pt_mutex);
            sem_post(&shared.pt_empty);
            if (type == GasEngine)
                sem_post(&shared.gas_sem);
            else
                sem_post(&shared.hybrid_sem);
            break;
        }
        
        // Enqueue the new powertrain.
        shared.pt_queue[shared.pt_rear] = type;
        shared.pt_rear = (shared.pt_rear + 1) % SharedData::PT_QUEUE_SIZE;
        shared.pt_count++;
        shared.produced[type]++;
        shared.inAssemblyQueue[type]++;
        shared.produced_total++;
        
        // Log production event.
        PowertrainAdded pa;
        pa.type = type;
        pa.produced = shared.produced;
        pa.inAssemblyQueue = shared.inAssemblyQueue;
        log_added_powertrain(pa);
        
        pthread_mutex_unlock(&shared.pt_mutex);
        sem_post(&shared.pt_full);
    }
    return NULL;
}
