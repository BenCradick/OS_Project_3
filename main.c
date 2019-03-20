#include <stdio.h>
#include <limits.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "shared_memory.h"


int getLineCount(FILE*);
int getLengthOfDigit(int);
int main(int argc, char** argv) {

    int childCount = 0;
    const int MAX_CHILDREN = 20;

    char* inputFile = argv[1];
    int rows;

    int status;
    pid_t pid;
    int count = 0;

    FILE* inputStream;

    inputStream =  fopen(inputFile, "r");
    rows = getLineCount(inputStream);

    SharedMemory sharedMemory = init(&sharedMemory, rows);

    char (*words)[rows][LINE_MAX] = NULL;

    char line[LINE_MAX];

    words = shmat(sharedMemory.shmid, (void*)0, 0);

    while(fgets(line, LINE_MAX, inputStream) != NULL)
    {
        strcpy((*words)[count], line);
        count++;
    }

    for(int i = 0; i < count; i++){
        if(childCount >= MAX_CHILDREN)
        {
            wait(&status);
        }
        pid = fork();
        if( pid == 0)
        {
            fprintf(stdout, "child process pre exec\n");
            char index[getLengthOfDigit(i)];
            sprintf(index, "%d", i);

            char size[getLengthOfDigit(rows)];
            sprintf(size, "%d", rows);

            char* args[4] = {"./palin", index, size, (char*)0 };
            if(execv("./palin", args) == -1)
            {
                perror("Failed to exec palin: ");
                return EXIT_FAILURE;
            }
        }
        else{
            fprintf(stdout, "increment child\n");
            childCount++;
        }
    }
    while(childCount > 0){
        if(wait(&status) > 0)
        {
            childCount--;
        }
    }
    sem_close(sharedMemory.noPalinSemaphore);
    sem_close(sharedMemory.palinSemaphore);
    shmdt(words);
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
int getLengthOfDigit(int digit){
    int length = 0;
    do{
        length++;
        digit /= 10;
    }while(digit != 0);
    return length;
}

