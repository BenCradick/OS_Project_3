//
// Created by Ben Cradick on 2019-03-18.
//

#include <stdlib.h>
#include <limits.h>
#include <sys/shm.h>
#include "shared_memory.h"

int main(int argc, char** argv){
    int index = (int) strtol(argv[1], NULL, 10);
    int row = (int) strtol(argv[2], NULL, 10);

    SharedMemory sharedMemory = init(&sharedMemory, row);

    char (*words)[row][LINE_MAX];
    words = shmat(sharedMemory.shmid, (void*)0, 0);


}