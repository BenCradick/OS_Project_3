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

SharedMemory init(SharedMemory *this, int size)
{
    this->key = ftok("shared_memory.c", 1);




    this->shmid = shmget(this->key, size * LINE_MAX, 0666 | IPC_CREAT);
    if(errno != 0){
        perror("Failed to obtain shared memory id: ");
    }



    this->noPalinFile = "nopalin.out";
    this->palinFile = "palin.out";

    this->palinSemaphore = sem_open(this->palinFile, O_CREAT);
    this->noPalinSemaphore = sem_open(this->noPalinFile, O_CREAT);

    return *this;
}
