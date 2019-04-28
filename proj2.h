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
        * Lector            : Prof. Ing. Tomáš Vojnar, Ph.D.
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
*                     (W - maximum number of miliseconds, after which the person comes back to queue)
*                     (C - Capacity of persons to be on mole_queue at once)
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
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

//Macros
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define FORK_PROBLEM return errorMsg("Fork problem")

#define semSIZE sizeof(sem_t)
#define shmSIZE sizeof(shm_dat_t)
#define shmKEY "/xjavor20-ios-proj2"
#define semHQUEUE "/xjavor20-ios-proj2-hackers-queue"
#define semSQUEUE "/xjavor20-ios-proj2-serfs-queue"
#define semBOAT "/xjavor20-ios-proj2-boat"
#define semMUTEX "/xjavor20-ios-proj2-mutex"
#define semMQUEUE "/xjavor20-ios-proj2-mole-queue"
#define semWRITING "/xjavor20-ios-proj2-writing"
#define semLEAVEBOAT "/xjavor20-ios-proj2-leaving-boat"
#define semCAPLEAVEBOAT "/xjavor20-ios-proj2-captain-leaving-boat"

#define NUMBER_OF_ARGUMENTS 7
#define RANDOMSLEEP(max_time) {if (max_time != 0) usleep(rand() % (max_time + 1));}

/// Shared memory for the integers
typedef struct
{
     int A;    // Serial number of action
     int NH;   // Number of Hackers on the mole_queue
     int NS;   // Number of Serfs on the mole_queue
     char *type;    //HACK or SERF
} shm_dat_t;


//Globals
shm_dat_t *shared_mem;
int mole_queue_capacity;
FILE *filename;
int arguments[NUMBER_OF_ARGUMENTS];
pid_t hackers_PID;
pid_t serfs_PID;

// Semaphores pointers
sem_t *sem_queue_hackers;   // Queue where the Hackers wait for the boat
sem_t *sem_queue_serfs;   // Queue where the Serfs wait for the boat
sem_t *sem_boat_barrier;   // Boat for the trip
sem_t *sem_mutex;   // Protecting mutex between Hackers and Serfs
sem_t *sem_queue_mole;   // Mole queue of waiting members
sem_t *sem_mutex_writing;      // Only one process can write at the time
sem_t *sem_leaving_boat;       // Leaving boat
sem_t *sem_leaving_captain;    // Captain can leave

/**
     * @brief Prints the error message on stderr
     * @param msg Error message
     * @return 1 as fail
     */
int errorMsg(char *msg);

/**
     * @brief Parsing arguments from argv to arguments array 
     * @param argc Number of arguments
     * @param argv Array of arguments
     * @param arguments Integer array where you want the arguments
     * from input to be stored
     * @return 1 if fails, 0 if success
     */
int gettingInputNumbers(int argc, char *argv[], int *argumets);

/**
     * @brief Checking for all conditions to be met
     * @param argv Integer array of arguments
     * @return 1 if fails, 0 if success
     */
int inputControl(int *argv);

/**
     * @brief Generating the serfs and hacker
     * @param P number of generated 'hackers' and 'serfs'
     * @return 1 if fails, 0 if success
     */ 
int spawn(int P);

/**
     * @brief Map shared memory and open semaphores
     */
void sInit();

/**
     * @brief Unmap shared memory and close semaphores
     */
void sDestroy();

/**
     * @brief Code for each hacker, and for Captian_hacker too
     * @param identificator Identificator
     */ 
void hackers(int identificator);

/**
     * @brief Code for each serf, and for Captian_serf too
     * @param identificator Identificator
     */
void serfs(int identificator);

/**
     * @brief Capitian signals the start of the trip
     * @param type Type of the process (HACK or SERF)
     * @param identificator Identificator
     */
void startTrip(char *type, int identificator);

/**
     * @brief Stuff that every process have to do when first opens his eyes
     * @param type Type of the process (HACK or SERF)
     * @param identificator Identificator
     */
void beeingBorn(char *type, int identificator);

/**
     * @brief Enters the mole
     * @param type Type of the process (HACK or SERF)
     * @param identificator Identificator
     */ 
void entersMole(char *type, int identificator);

/**
     * @brief Leaving boat for member
     * @param type Type of the process (HACK or SERF)
     * @param identificator Identificator
     */  
void leavingBoat(char *type, int identificator);

/**
     * @brief Leaving boat for captain
     * @param type Type of the process (HACK or SERF)
     * @param identificator Identificator
     */  
void captainLeaves(char *type, int identificator);

/**
     * @brief Writing the output
     * @param type Type of the process (HACK or SERF)
     * @param action What action is beeing done
     * @param identificator Identificator
     * @param counter Write count of hackers and serfs on mole or not
     */  
void writeOut(char *type, char *action, int identificator, bool counter);
#endif // PROJ2_H