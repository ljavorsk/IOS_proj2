/*
|***************************************************************************;
* Project           : River Crossing Problem (Semaphores)
        *
        * Program name      : proj2.h
        *
        * Author            : Lukas Javorsky (xjavor20@stud.fit.vutbr.cz)
        *
        * Subject           : IOS, project 2 "Práce s procesy (Synchronizace)"
        *
        * Lector            : Ing. Aleš Smrčka, Ph.D.
        *
        * University        : Faculty of Information Technology VUT Brno
        *
        * Date created      : 20190420
*
* Purpose           : River Crossing Problem implementation, semaphores are used for synchronization
*
* Usage             : Creats the P*2 number of persons and takes them on trip by 4.
*                     After R miliseconds comes back, and the 4 persons leave and another 4
*                     takes their places.
*                     It ends when everyone have been on trip and left the boat
*
* Execute           : ./proj2 P H S R W C
*                     (P - number of generated 'hackers' and 'serfs')
*                     (H - maximum number of miliseconds, to generate new 'hacker')
*                     (S - maximum number of miliseconds, to generate new 'serfs')
*                     (R - maximum number of miliseconds, how long the trip takes)
*                     (W - maximum number of miliseconds, after which the person comes back to mole)
*                     (C - Capacity of persons to be on mole at once)
*
|***************************************************************************;
 */

#ifndef PROJ2_H
#define PROJ2_H

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

//Macros
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define NUMBER_OF_ARGUMENTS 7
#define MMAP(ptr) {(ptr) = mmap(NULL, sizeof(*(ptr)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);}
#define RANDOMSLEEP(max_time) {if (max_time != 0) sleep(rand() % max_time);}
#define UNMAP(ptr) {munmap((ptr), sizeof((ptr)));}

//Globals
int *shared_mem = NULL;
sem_t *semaphore = NULL;
FILE *file;

/**
     * @brief Prints the error message on stderr
     * @param msg Error message
     * @return 1 as fail
     */
int ErrorMsg(char *msg);

/**
     * @brief Parsing arguments from argv to arguments array 
     * @param argc Number of arguments
     * @param argv Array of arguments
     * @param arguments Integer array where you want the arguments
     * from input to be stored
     * @return 1 if fails, 0 if success
     */
int GettingInputNumbers(int argc, char *argv[], int *argumets);

/**
     * @brief Checking for all conditions to be met
     * @param argv Integer array of arguments
     * @return 1 if fails, 0 if success
     */
int InputControl(int *argv);

#endif // PROJ2_H