//
// Created by Ben Cradick on 2019-03-18.
//

#include <stdlib.h>
#include <limits.h>
#include <sys/shm.h>
#include <memory.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "shared_memory.h"

int  isPalindrome(char* word);
int main(int argc, char** argv)
{
    int index = (int) strtol(argv[1], NULL, 10);
    int row = (int) strtol(argv[2], NULL, 10);

    int palindrome;

    FILE* out;

    time_t t;
    unsigned int sleepDuration;

    SharedMemory sharedMemory = init(&sharedMemory, row);

    char (*words)[row][LINE_MAX];
    words = shmat(sharedMemory.shmid, (void*)0, 0);

    srand((unsigned)time(&t));
    sleepDuration = (unsigned) rand() % 10;

    palindrome = isPalindrome((*words)[index]);

    for(int i = 0; i < 5; i++) // in this loop because its in the project description, really doesn't serve a purpose
    {
        sleep(sleepDuration); //did this because its was in project description not sure of purpose

        fprintf(stderr,"About to enter critical section: %s\n", ctime(&t));

        if(palindrome)
        {
            sleep(2);

            sem_wait(sharedMemory.palinSemaphore);

            fprintf(stderr, "In critical section for palin.out: %s\n", ctime((&t)));

            sharedMemory.palinStream = fopen(sharedMemory.palinFile, "a+");

            fprintf(sharedMemory.palinStream, "PID: %d, Index: %d, String: %s", getpid(), index, (*words)[index]);

            fclose(sharedMemory.palinStream);

            sleep(2);

            fprintf(stderr, "Exiting critical section for palin.out: %s\n", ctime(&t));

            sem_post(sharedMemory.palinSemaphore);

            sem_unlink(sharedMemory.palinFile);

            exit(EXIT_SUCCESS);
        }
        else
        {
            sleep(2);

            sem_wait(sharedMemory.noPalinSemaphore);

            fprintf(stderr, "In critical section for nopalin.out: %s\n", ctime(&t));

            sharedMemory.noPalinStream = fopen(sharedMemory.noPalinFile, "a+");

            fprintf(sharedMemory.noPalinStream, "PID: %d, Index: %d, String: %s", getpid(), index, (*words)[index]);

            fclose(sharedMemory.noPalinStream);

            sleep(2);

            fprintf(stderr, "Exiting critical section for nopalin.out: %s\n", ctime(&t));

            sem_post(sharedMemory.noPalinSemaphore);

            sem_unlink(sharedMemory.noPalinFile);

            exit(EXIT_SUCCESS);
        }
    }




}
int isPalindrome(char* word)
{
    int length = (int) strlen(word) - 2; // subtract -1 to adjust for indexing at 0 and -1 to ignore the \n character
    int start = 0;
    while(length > start)
    {
        if(word[start++] != word[length--])
        {
            return 0;
        }
    }
    return 1;
};