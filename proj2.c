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
    fprintf(stderr, "%s\n", msg);
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