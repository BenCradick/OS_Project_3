//
// Created by Ben Cradick on 2019-03-17.
//
#include "shared_memory.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
SharedMemory init(SharedMemory *this, int size)
{
    this->key = ftok("shared_memory.c", 1);
    if(errno!= 0)
    {
        perror("Failed to generate semaphore key:");
        exit(EXIT_FAILURE);
    }




    this->shmid = shmget(this->key, (size_t) size * LINE_MAX, 0666 | IPC_CREAT);
    if(errno != 0)
    {
        perror("Failed to obtain shared memory id: ");
        exit(EXIT_FAILURE);
    }



    this->noPalinFile = "nopalin.out";
    this->palinFile = "palin.out";

    this->palinSemaphore = sem_open(this->palinFile, O_CREAT, 0666, 1);
    if(errno != 0){
        perror("Unable to initialize semaphore to protect palin.out");
        exit(EXIT_FAILURE);
    }
    this->noPalinSemaphore = sem_open(this->noPalinFile, O_CREAT, 0666, 1);
    if(errno != 0){
        perror("Unable to initialize semaphore to protect nopalin.out");
        exit(EXIT_FAILURE);
    }

    return *this;
}
