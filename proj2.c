/*
|***************************************************************************;
* Project           : River Crossing Problem (Semaphores)
        *
        * Program name      : proj2.c
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

#include "proj2.h"

int main(int argc, char *argv[])
{
    int arguments[7];
    if (GettingInputNumbers(argc, argv, arguments) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (InputControl(arguments) == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int ErrorMsg(char *msg)
{
    fprintf(stderr, RED"%s\n"RESET, msg);
    return EXIT_FAILURE;
}

int GettingInputNumbers(int argc, char *argv[], int *argumets)
{    
    if (argc != NUMBER_OF_ARGUMENTS)
        return ErrorMsg("The number of arguments have to be 6");
        
    double tmp_arguments[7];
    char *pEnd;

    for(short i = 1; i < NUMBER_OF_ARGUMENTS; i++)
    {
        tmp_arguments[i] = strtof(argv[i], &pEnd);
        // Firstly looking for any non-number characters, then for floats
        if (strcmp(pEnd,"") != 0 || tmp_arguments[i] != (int)tmp_arguments[i])
            return ErrorMsg("The arguments have to be integers");
        argumets[i] = (int) tmp_arguments[i];
    }

    return 0;
}

int InputControl(int *argv)
{
    if (argv[1] < 2 || (argv[1] % 2) != 0)
        return ErrorMsg("P argument have to be bigger than 2 and even");

    for(short i = 2; i < 5; i++)
    {
        if (argv[i] < 0 || argv[i] > 2000)
            return ErrorMsg("H,S,R  arguments have to be in interval <0, 2000> !");
    }
    
    if (argv[5] < 20 || argv[5] > 2000)
        return ErrorMsg("W argument have to be in interval <20, 2000> !");

    if (argv[6] < 5)
        return ErrorMsg("C argument have to be bigger than 5");

    return 0;
}