#ifndef PROJ2_H
#define PROJ2_H

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define NUMBER_OF_ARGUMENTS 7

#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

/**
     * @brief Prints the error message on stderr
     * @param msg Error message
     * @return 1 as fail
     */
int ErrorMsg(char *msg);

/**
     * @brief Checking for all conditions to be met
     * @param argc Number of arguments
     * @param argv Array of arguments
     * @return
     */
int InputControl(int argc, char argv[]);

#endif // PROJ2_H