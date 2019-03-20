//
// Created by Ben Cradick on 2019-03-17.
//

#ifndef CRADICK_3_SHARED_MEMORY_H
#define CRADICK_3_SHARED_MEMORY_H

#include <stdio.h>
#include <semaphore.h>


struct SharedMemory;

typedef struct SharedMemory {
    char *palinFile;
    char *noPalinFile;

    FILE* palinStream;
    FILE* noPalinStream;

    sem_t *palinSemaphore;
    sem_t *noPalinSemaphore;

    key_t key;


    int shmid;



}SharedMemory;

SharedMemory init(SharedMemory *this, int size);
#endif //CRADICK_3_SHARED_MEMORY_H
