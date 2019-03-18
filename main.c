#include <stdio.h>
#include <limits.h>
#include <sys/shm.h>
#include <string.h>
#include "shared_memory.h"
int getLineCount(FILE*);
int main(int argc, char** argv) {

    int childCount = 0;
    const int MAX_CHILDREN = 20;

    char* inputFile = argv[1];
    int rows;

    int count = 0;

    FILE* inputStream;

    inputStream =  fopen(inputFile, "r");
    rows = getLineCount(inputStream);

    SharedMemory sharedMemory = init(&sharedMemory, rows);

    // marks the newly created semaphore as open
    sem_post(sharedMemory.noPalinSemaphore);
    sem_post(sharedMemory.palinSemaphore);

    char (*words)[rows][LINE_MAX];
    char line[LINE_MAX];

    words = shmat(sharedMemory.shmid, (void*)0, 0);

    while(fgets(line, LINE_MAX, inputStream) != NULL)
    {
        strcpy((*words)[count], line);
        count++;
    }
    for(int i = 0; i < count; i++){
        fprintf(stdout, "%s", (*words)[i]);
    }
    sem_close(sharedMemory.noPalinSemaphore);
    sem_close(sharedMemory.palinSemaphore);
    //shmdt(sharedMemory.words);
    shmctl(sharedMemory.shmid, IPC_RMID, NULL);

    return 0;
}
int getLineCount(FILE* fp){
    int count = 0;
    char ch;
    while(!feof(fp)){
        ch = (char)fgetc(fp);
        if(ch == '\n'){
            count++;
        }
    }
    rewind(fp);
    return count;
}

