#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/mman.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char** argv) {

    char* commandName = argv[1];
    printf("Command %s execution \n", commandName);


    char commandPath[20]; 
    strcpy(commandPath, "/bin/");
    strcat(commandPath, commandName);
    printf("Command path is %s \n", commandPath);

    struct timeval startTime;
    size_t size = sizeof(struct timeval); 

    int fd[2];

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

 
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Error forking.");
        return 1;
    } 

    if (pid > 0) {
        //this is parent process

        close(fd[WRITE_END]);

        int status;
        wait(&status);

        read(fd[READ_END], &startTime, size);

        struct timeval endTime;
        gettimeofday(&endTime, NULL);

        printf("PARENT START TIME %ld \n", startTime.tv_usec);
        printf("PARENT END TIME %ld \n", endTime.tv_usec);

        __suseconds_t msPassed = (endTime.tv_usec - startTime.tv_usec)/1000;
        printf("Command %s execution took %ld ms \n", commandName, msPassed);

        if (status == 0) {
            printf("Command %s execution success", commandName);
        }

    } else {
        //this is child process
        close(fd[READ_END]);

        gettimeofday(&startTime, NULL);
        printf("CHILD START TIME %ld \n", startTime.tv_usec);

        write(fd[WRITE_END], &startTime, size);

        execl(commandPath, commandName, NULL);
    }

    return 0;
}