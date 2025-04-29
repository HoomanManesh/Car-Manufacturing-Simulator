#include "shared_datastruc.h"
#include <semaphore.h>
#include <pthread.h>

SharedData shared;

void init_shared_data(int production_limit) {
    shared.pt_front = 0;
    shared.pt_rear = 0;
    shared.pt_count = 0;
    shared.ch_front = 0;
    shared.ch_rear = 0;
    shared.ch_count = 0;
    
    shared.production_limit = production_limit;
    shared.produced_total = 0;
    shared.powertrain_consumed_total = 0;
    shared.chassis_consumed = 0;
    
    for (int i = 0; i < PowertrainTypeN; i++) {
        shared.produced[i] = 0;
        shared.inAssemblyQueue[i] = 0;
    }
    for (int i = 0; i < ChassisRobotTypeN; i++) {
        for (int j = 0; j < PowertrainTypeN; j++) {
            shared.consumed[i][j] = 0;
        }
    }
    
    sem_init(&shared.pt_empty, 0, SharedData::PT_QUEUE_SIZE);
    sem_init(&shared.pt_full, 0, 0);
    pthread_mutex_init(&shared.pt_mutex, NULL);
    
    sem_init(&shared.gas_sem, 0, 12);
    sem_init(&shared.hybrid_sem, 0, 8);
    
    sem_init(&shared.ch_empty, 0, SharedData::CH_QUEUE_SIZE);
    sem_init(&shared.ch_full, 0, 0);
    pthread_mutex_init(&shared.ch_mutex, NULL);
}

void destroy_shared_data() {
    sem_destroy(&shared.pt_empty);
    sem_destroy(&shared.pt_full);
    pthread_mutex_destroy(&shared.pt_mutex);
    sem_destroy(&shared.gas_sem);
    sem_destroy(&shared.hybrid_sem);
    
    sem_destroy(&shared.ch_empty);
    sem_destroy(&shared.ch_full);
    pthread_mutex_destroy(&shared.ch_mutex);
}
