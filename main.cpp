#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "shared_datastruc.h"
#include "powertrain_producer.h"
#include "powertrain_consumer.h"
#include "powered_chassis.h"
#include "car_assemble.h"  // Provides powertrain_producerNames and powertrain_consumerNames arrays
#include "log.h"

// Global delay variables (milliseconds)
int sleep_g = 0;  // GasEngine production delay
int sleep_h = 0;  // HybridEngine production delay
int sleep_t = 0;  // Titano consumption delay
int sleep_m = 0;  // MegaForce consumption delay
int sleep_r = 0;  // RoboMount delay

int main(int argc, char* argv[]) {
    int production_limit = 100;  // Default production limit
    sleep_g = 0; sleep_h = 0; sleep_t = 0; sleep_m = 0; sleep_r = 0;
    
    // Parse command-line arguments.
    int opt;
    while ((opt = getopt(argc, argv, "n:g:h:t:m:r:")) != -1) {
        switch(opt) {
            case 'n': production_limit = std::atoi(optarg); break;
            case 'g': sleep_g = std::atoi(optarg); break;
            case 'h': sleep_h = std::atoi(optarg); break;
            case 't': sleep_t = std::atoi(optarg); break;
            case 'm': sleep_m = std::atoi(optarg); break;
            case 'r': sleep_r = std::atoi(optarg); break;
            default:
                std::cerr << "Usage: " << argv[0]
                          << " -n <production_limit> -g <gas_ms> -h <hybrid_ms> -t <titano_ms> -m <megaforce_ms> -r <robomount_ms>\n";
                return 1;
        }
    }
    
    init_shared_data(production_limit);
    
    pthread_t producer_threads[2];
    pthread_t consumer_threads[2];
    pthread_t chassis_thread;
    
    // Create producer threads.
    PowertrainType gas = GasEngine;
    PowertrainType hybrid = HybridEngine;
    pthread_create(&producer_threads[0], NULL, powertrain_producer, &gas);
    pthread_create(&producer_threads[1], NULL, powertrain_producer, &hybrid);
    
    // Create powertrain consumer threads.
    ChassisRobotType titano = Titano;
    ChassisRobotType megaforce = MegaForce;
    pthread_create(&consumer_threads[0], NULL, powertrain_consumer, &titano);
    pthread_create(&consumer_threads[1], NULL, powertrain_consumer, &megaforce);
    
    // Create chassis consumer thread (RoboMount).
    pthread_create(&chassis_thread, NULL, powered_chassis_consumer, NULL);
    
    // Wait for producer threads to finish.
    pthread_join(producer_threads[0], NULL);
    pthread_join(producer_threads[1], NULL);
    
    // Insert poison pills into the powertrain queue (one for each consumer).
    for (int i = 0; i < 2; i++) {
        sem_wait(&shared.pt_empty);
        pthread_mutex_lock(&shared.pt_mutex);
        shared.pt_queue[shared.pt_rear] = (PowertrainType)(-1);
        shared.pt_rear = (shared.pt_rear + 1) % SharedData::PT_QUEUE_SIZE;
        shared.pt_count++;
        pthread_mutex_unlock(&shared.pt_mutex);
        sem_post(&shared.pt_full);
    }
    
    // Wait for powertrain consumer threads to finish.
    pthread_join(consumer_threads[0], NULL);
    pthread_join(consumer_threads[1], NULL);
    
    // Insert a poison pill into the chassis queue.
    sem_wait(&shared.ch_empty);
    pthread_mutex_lock(&shared.ch_mutex);
    shared.ch_queue[shared.ch_rear] = "POISON";
    shared.ch_rear = (shared.ch_rear + 1) % SharedData::CH_QUEUE_SIZE;
    shared.ch_count++;
    pthread_mutex_unlock(&shared.ch_mutex);
    sem_post(&shared.ch_full);
    
    // Wait for the chassis consumer thread to finish.
    pthread_join(chassis_thread, NULL);
    
    // Print final summary.
    std::cout << "REQUEST REPORT\n----------------------------------------\n";
    std::cout << "GAS powertrain producer generated " 
              << shared.produced[GasEngine] << " requests\n";
    std::cout << "HYBRID powertrain producer generated " 
              << shared.produced[HybridEngine] << " requests\n";
    std::cout << "Titano consumed " 
              << shared.consumed[Titano][GasEngine] << " GAS + " 
              << shared.consumed[Titano][HybridEngine] << " HYBRID = " 
              << (shared.consumed[Titano][GasEngine] + shared.consumed[Titano][HybridEngine])
              << " total\n";
    std::cout << "MegaForce consumed " 
              << shared.consumed[MegaForce][GasEngine] << " GAS + " 
              << shared.consumed[MegaForce][HybridEngine] << " HYBRID = " 
              << (shared.consumed[MegaForce][GasEngine] + shared.consumed[MegaForce][HybridEngine])
              << " total\n";
    
    destroy_shared_data();
    
    return 0;
}
