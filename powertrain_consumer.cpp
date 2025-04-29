#include "powertrain_consumer.h"
#include "shared_datastruc.h"
#include "log.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string>

extern int sleep_t; // Titano delay (ms)
extern int sleep_m; // MegaForce delay (ms)

static void msleep(int ms) {
    usleep(ms * 1000);
}

void* powertrain_consumer(void* arg) {
    ChassisRobotType consumer = *(ChassisRobotType*)arg;
    int sleep_time = (consumer == Titano) ? sleep_t : sleep_m;
    
    while (true) {
        sem_wait(&shared.pt_full);
        pthread_mutex_lock(&shared.pt_mutex);
        
        // If a poison pill (-1) is encountered, remove it and exit.
        if (shared.pt_queue[shared.pt_front] == (PowertrainType)(-1)) {
            shared.pt_front = (shared.pt_front + 1) % SharedData::PT_QUEUE_SIZE;
            shared.pt_count--;
            pthread_mutex_unlock(&shared.pt_mutex);
            sem_post(&shared.pt_empty);
            break;
        }
        
        // If production is done and queue is empty, exit.
        if (shared.produced_total >= shared.production_limit && shared.pt_count == 0) {
            pthread_mutex_unlock(&shared.pt_mutex);
            break;
        }
        
        // Otherwise, dequeue a powertrain item.
        PowertrainType engine = shared.pt_queue[shared.pt_front];
        shared.pt_front = (shared.pt_front + 1) % SharedData::PT_QUEUE_SIZE;
        shared.pt_count--;
        shared.inAssemblyQueue[engine]--;
        shared.powertrain_consumed_total++;
        
        PowertrainRemoved pr;
        pr.consumer = consumer;
        pr.type = engine;
        pr.consumed = shared.consumed[consumer];
        pr.inAssemblyQueue = shared.inAssemblyQueue;
        log_removed_powertrain(pr);
        
        pthread_mutex_unlock(&shared.pt_mutex);
        sem_post(&shared.pt_empty);
        
        if (engine == GasEngine)
            sem_post(&shared.gas_sem);
        else
            sem_post(&shared.hybrid_sem);
        
        msleep(sleep_time);
        shared.consumed[consumer][engine]++;
        
        std::string chassis = std::string(powertrain_consumerNames[consumer]) +
            ((engine == GasEngine) ? powertrain_producerNames[GasEngine] : powertrain_producerNames[HybridEngine]);
        
        // Enqueue the chassis.
        sem_wait(&shared.ch_empty);
        pthread_mutex_lock(&shared.ch_mutex);
        
        shared.ch_queue[shared.ch_rear] = chassis;
        shared.ch_rear = (shared.ch_rear + 1) % SharedData::CH_QUEUE_SIZE;
        shared.ch_count++;
        
        log_added_poweredchassis(chassis.c_str(), shared.ch_count);
        
        pthread_mutex_unlock(&shared.ch_mutex);
        sem_post(&shared.ch_full);
    }
    return NULL;
}
