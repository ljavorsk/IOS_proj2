#include "proj2.h"

int main(int argc, char *argv[])
{
    if (InputControl(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int ErrorMsg(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    return EXIT_FAILURE;
}

int InputControl(int argc, char argv[])
{
    if (argc != NUMBER_OF_ARGUMENTS)
        return ErrorMsg("The number of arguments have to be 7");

    if (argv[1] < 2 || (argv[1] % 2) != 0)
        return ErrorMsg("P argument have to be bigger than 2 and even");

    if (argv[2] < 0 || argv[2] > 2000 && argv[3] < 0 || argv[3] > 2000 && argv[4] < 0 || argv[4] > 2000)
        return ErrorMsg("H,S,R  arguments have to be in interval <0, 2000> !");

    if (argv[5] < 20 || argv[5] > 2000)
        return ErrorMsg("W argument have to be in interval <20, 2000> !");

    if (argv[6] < 5)
        return ErrorMsg("C argument have to be bigger than 5");

    for (size_t i = 0; i < NUMBER_OF_ARGUMENTS; i++)
    {
        if (argv[i] != (int)argv[i])
            return ErrorMsg("The arguments have to be integers");
    }

    return 0;
}