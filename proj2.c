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
*                     (W - maximum number of miliseconds, after which the person comes back to mole)
*                     (C - Capacity of persons to be on mole_queue at once)
*
|***************************************************************************;
 */

#include "proj2.h"

int main(int argc, char *argv[])
{
    if (gettingInputNumbers(argc, argv, arguments) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (inputControl(arguments) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if ((filename = fopen("proj2.out", "w")) == NULL){
        return errorMsg("File openning problem");
    };

    if (spawn(arguments[1]) == EXIT_FAILURE)
        return EXIT_FAILURE;
    fclose(filename);
    return EXIT_SUCCESS;
}

int errorMsg(char *msg)
{
    fprintf(stderr, RED"%s\n"RESET, msg);
    return EXIT_FAILURE;
}

int gettingInputNumbers(int argc, char *argv[], int *argumets)
{    
    if (argc != NUMBER_OF_ARGUMENTS)
        return errorMsg("The number of arguments have to be 6");
        
    double tmp_arguments[NUMBER_OF_ARGUMENTS];
    char *pEnd;

    for(short i = 1; i < NUMBER_OF_ARGUMENTS; i++)
    {
        tmp_arguments[i] = strtof(argv[i], &pEnd);
        // Firstly looking for any non-number characters, then for floats
        if (strcmp(pEnd,"") != 0 || tmp_arguments[i] != (int)tmp_arguments[i])
            return errorMsg("The arguments have to be integers");
        argumets[i] = (int) tmp_arguments[i];
    }
    mole_queue_capacity = argumets[6];

    return EXIT_SUCCESS;
}

int inputControl(int *argv)
{
    if (argv[1] < 2 || (argv[1] % 2) != 0)
        return errorMsg("P argument have to be bigger than 2 and even");

    for(short i = 2; i < 5; i++)
    {
        if (argv[i] < 0 || argv[i] > 2000)
            return errorMsg("H,S,R  arguments have to be in interval <0, 2000> !");
    }
    
    if (argv[5] < 20 || argv[5] > 2000)
        return errorMsg("W argument have to be in interval <20, 2000> !");

    if (mole_queue_capacity < 5)
        return errorMsg("Capacity argument have to be bigger than 5");

    return EXIT_SUCCESS;
}

int spawn(int P)
{
    int pid;
    pid_t hackers_children [P];
    pid_t serfs_children [P];

    sInit();

    if ((pid = fork()) < 0) FORK_PROBLEM;

    if (pid == 0){  // Child Hacker Spawner
        for(int i = 0; i < P; i++)
        {
            if ((hackers_children[i] = fork()) < 0) FORK_PROBLEM;

            if (hackers_children[i] == 0){  // Child Hacker
                hackers(i+1);
                _Exit(EXIT_SUCCESS);
            }
            RANDOMSLEEP(arguments[3]);
        }
        for (int i = 0; i < P; i++)
        {
            waitpid(hackers_children[i], NULL, 0);  // Waits for all hacker children
        }
        
        _Exit(EXIT_SUCCESS);
    }else { // Parent Hacker
        hackers_PID = pid;
        if ((pid = fork()) < 0) FORK_PROBLEM;

        if (pid == 0){  // Child Serf Spawner
            for(int i = 0; i < P; i++)
            {
                if ((serfs_children[i] = fork()) < 0) FORK_PROBLEM;

                if (serfs_children[i] == 0){  // Child Serf
                    serfs(i+1);
                    _Exit(EXIT_SUCCESS);
                }
                RANDOMSLEEP(arguments[4]);
            }
            for (int i = 0; i < P; i++)
            {
                waitpid(serfs_children[i], NULL, 0);    // Waits for all serf children
            }
            _Exit(EXIT_SUCCESS);
        }else {  // Parent Serf
            serfs_PID = pid;
        }
    }
    waitpid(hackers_PID, NULL, 0);
    waitpid(serfs_PID, NULL, 0);

    sDestroy();
    return EXIT_SUCCESS;
}

void sInit() {
    
    shared_mem = (shm_dat_t*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_queue_hackers = sem_open(semHQUEUE, O_CREAT | O_EXCL, 0666, 0);
    sem_queue_serfs = sem_open(semSQUEUE, O_CREAT | O_EXCL, 0666, 0);
    sem_boat_barrier = sem_open(semBOAT, O_CREAT | O_EXCL, 0666, 4);
    sem_mutex = sem_open(semMUTEX, O_CREAT | O_EXCL, 0666, 1);
    sem_queue_mole = sem_open(semMQUEUE, O_CREAT | O_EXCL, 0666, 1);
    sem_mutex_writing = sem_open(semWRITING, O_CREAT | O_EXCL, 0666, 1);
    sem_leaving_boat = sem_open(semLEAVEBOAT, O_CREAT | O_EXCL, 0666, 0);
    sem_leaving_captain = sem_open(semCAPLEAVEBOAT, O_CREAT | O_EXCL, 0666, 0);

    setbuf(filename,NULL);  // Needs to be done, because children want to write
    
}

void sDestroy() {
    munmap(shared_mem, shmSIZE);

    sem_close(sem_queue_hackers);
    sem_close(sem_queue_serfs);
    sem_close(sem_boat_barrier);
    sem_close(sem_mutex);
    sem_close(sem_queue_mole);
    sem_close(sem_mutex_writing);
    sem_close(sem_leaving_boat);
    sem_close(sem_leaving_captain);

    sem_unlink(semHQUEUE);
    sem_unlink(semSQUEUE);
    sem_unlink(semBOAT);
    sem_unlink(semMUTEX);
    sem_unlink(semMQUEUE);
    sem_unlink(semWRITING);
    sem_unlink(semLEAVEBOAT);
    sem_unlink(semCAPLEAVEBOAT);
}

void hackers(int identificator){
    bool is_Captian;
    char *type = "HACK";
    
    beeingBorn(type, identificator);
    sem_wait(sem_mutex);
    
    shared_mem->NH++;
    entersMole(type, identificator);
    if (shared_mem->NH == 4){
        for(int i = 0; i < 4; i++) sem_post(sem_queue_hackers); // Let 4 hackers pass
        shared_mem->NH = 0; // 0 Hackers in the queue
        is_Captian = true;
    }else if (shared_mem->NH == 2 && shared_mem->NS >= 2) {
        for(int i = 0; i < 2; i++) sem_post(sem_queue_hackers); // Let 2 hackers pass
        for(int i = 0; i < 2; i++) sem_post(sem_queue_serfs);   // Let 2 serfs pass
        shared_mem->NH = 0;     // 0 Hackers in the queue
        shared_mem->NS -= 2;    // -2 Serfs in the queue
        is_Captian = true;
    }else {
        sem_post(sem_mutex);    // End of critical that only captain enters (and holds the mutex)
    }

    sem_wait(sem_queue_hackers);
    sem_wait(sem_boat_barrier);
    
    if (is_Captian){
        startTrip(type, identificator);
        captainLeaves(type, identificator);
        for(int i = 0; i < 4; i++) sem_post(sem_boat_barrier); // You can access the boat
        sem_post(sem_mutex);    // Captain also release mutex
    }else {
        sem_wait(sem_leaving_boat);
        leavingBoat(type, identificator);
        sem_post(sem_leaving_captain);
    }

}

void serfs(int identificator){
    bool is_Captian;
    char *type = "SERF";

    beeingBorn(type, identificator);
    sem_wait(sem_mutex);
    
    shared_mem->NS++;
    entersMole(type, identificator);
    if (shared_mem->NS == 4){
        for(int i = 0; i < 4; i++) sem_post(sem_queue_serfs); // Let 4 serfs pass
        shared_mem->NS = 0; // 0 Hackers in the queue
        is_Captian = true;
    }else if (shared_mem->NS == 2 && shared_mem->NH >= 2) {
        for(int i = 0; i < 2; i++) sem_post(sem_queue_serfs); // Let 2 serfs pass
        for(int i = 0; i < 2; i++) sem_post(sem_queue_hackers);   // Let 2 hackers pass
        shared_mem->NS = 0;     // 0 Serfs in the queue
        shared_mem->NH -= 2;    // -2 Hackers in the queue
        is_Captian = true;
    }else {
        sem_post(sem_mutex);    // End of critical that only captain enters (and holds the mutex)
    }

    sem_wait(sem_queue_serfs);
    sem_wait(sem_boat_barrier);
    
    if (is_Captian){
        startTrip(type, identificator);
        captainLeaves(type, identificator);
        for(int i = 0; i < 4; i++) sem_post(sem_boat_barrier); // You can access the boat
        sem_post(sem_mutex);    // Captain also release mutex
    }else {
        sem_wait(sem_leaving_boat);
        leavingBoat(type, identificator);
        sem_post(sem_leaving_captain);
    }
}

void startTrip(char *type, int identificator){
    writeOut(type,"boards", identificator, true);
    RANDOMSLEEP(arguments[5]);
    for(int i = 0; i < 3; i++) sem_post(sem_leaving_boat); // Leaving boat
    for(int i = 0; i < 3; i++) sem_wait(sem_leaving_captain); // Leaving captain
}

void beeingBorn(char *type, int identificator){
    writeOut(type,"starts", identificator, false);
}

void entersMole(char *type, int identificator){
    
    while(true){
        sem_wait(sem_queue_mole);
        if (shared_mem->NH + shared_mem->NS <= mole_queue_capacity){
            writeOut(type,"waits", identificator, true);
            sem_post(sem_queue_mole);
            break;
        }else
        {
            writeOut(type,"leaves queue", identificator, true);
            sem_post(sem_queue_mole);
            RANDOMSLEEP(arguments[5]);
            writeOut(type,"is back", identificator, false);
        }
    }
}

void leavingBoat(char *type, int identificator){
    writeOut(type, "member exits", identificator, true);
}

void captainLeaves(char *type, int identificator){
    writeOut(type, "captain exits", identificator, true);
}

void writeOut(char *type, char *action, int identificator, bool counter){
    sem_wait(sem_mutex_writing);

    shared_mem->A++;
    if (counter){
        if (strcmp(type,"HACK") == 0){
            fprintf(filename,"%d : %s %d : %s : %d : %d\n",shared_mem->A,type,identificator, action, shared_mem->NH, shared_mem->NS);
            fprintf(stdout,"%d : %s %d : %s : %d : %d\n",shared_mem->A,type,identificator, action, shared_mem->NH, shared_mem->NS);
        }else {
            fprintf(filename,"%d : %s %d : %s : %d : %d\n",shared_mem->A,type,identificator, action, shared_mem->NH, shared_mem->NS);
            fprintf(stdout,"%d : %s %d : %s : %d : %d\n",shared_mem->A,type,identificator, action, shared_mem->NH, shared_mem->NS);
        }
    }else {
        if (strcmp(type,"HACK") == 0){
            fprintf(filename,"%d : %s %d : %s\n",shared_mem->A,type,identificator, action);
            fprintf(stdout, "%d : %s %d : %s\n",shared_mem->A,type,identificator,action);
        }else {
            fprintf(filename,"%d : %s %d : %s\n",shared_mem->A,type,identificator,action);
            fprintf(stdout,"%d : %s %d : %s\n",shared_mem->A,type,identificator,action);
        }
    }
    
    sem_post(sem_mutex_writing);
}