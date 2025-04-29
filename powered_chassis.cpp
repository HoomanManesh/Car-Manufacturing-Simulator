#include "powered_chassis.h"
#include "shared_datastruc.h"
#include "log.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string>

extern int sleep_r;  // RoboMount delay (ms)

static void msleep(int ms) {
    usleep(ms * 1000);
}

void* powered_chassis_consumer(void* arg) {
    while (true) {
        sem_wait(&shared.ch_full);
        pthread_mutex_lock(&shared.ch_mutex);
        
        // If we encounter the poison pill "POISON", remove it and exit.
        if (shared.ch_queue[shared.ch_front] == "POISON") {
            shared.ch_front = (shared.ch_front + 1) % SharedData::CH_QUEUE_SIZE;
            shared.ch_count--;
            pthread_mutex_unlock(&shared.ch_mutex);
            sem_post(&shared.ch_empty);
            break;
        }
        
        std::string chassis = shared.ch_queue[shared.ch_front];
        shared.ch_front = (shared.ch_front + 1) % SharedData::CH_QUEUE_SIZE;
        shared.ch_count--;
        shared.chassis_consumed++;
        
        log_removed_poweredchassis(chassis.c_str(), shared.ch_count, shared.chassis_consumed);
        
        pthread_mutex_unlock(&shared.ch_mutex);
        sem_post(&shared.ch_empty);
        
        msleep(sleep_r);
    }
    return NULL;
}
