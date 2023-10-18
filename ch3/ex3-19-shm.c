#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{

    char *commandName = argv[1];
    printf("Command %s execution \n", commandName);

    char commandPath[20];
    strcpy(commandPath, "/bin/");
    strcat(commandPath, commandName);
    printf("Command path is %s \n", commandPath);

    const char *name = "current_time";
    int shmfd;

	const size_t size_SHM = 4096;
    size_t obj_size = sizeof(struct timespec);

    shmfd = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (shmfd == -1)
    {
        fprintf(stderr, "Error shm_open.");
        return 1; // Handle the error
    }

    if (ftruncate(shmfd, obj_size) == -1)
    {
        fprintf(stderr, "Error ftruncate.");
        return 1;
    }

    struct timespec *shm_ptr = mmap(NULL, obj_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    pid_t pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Error forking.");
        return 1;
    }

    if (pid > 0)
    {
        // this is parent process
        int status;
        wait(&status);

        struct timespec endTime;
        clock_gettime(CLOCK_MONOTONIC, &endTime);

        printf("PARENT START TIME %ld \n", shm_ptr->tv_nsec);
        printf("PARENT END TIME %ld \n", endTime.tv_nsec);

		long ns = (endTime.tv_nsec - shm_ptr->tv_nsec);
		long sec = (endTime.tv_sec - shm_ptr->tv_sec);
		printf("Execution took: %ld [s] %ld [ns] \n", sec, ns);

        munmap(shm_ptr, obj_size);
        close(shmfd);

        if (status == 0)
        {
            printf("Command %s execution success", commandName);
        }

        shm_unlink(name);
    }
    else
    {
        // this is child process
        clock_gettime(CLOCK_MONOTONIC, shm_ptr);
        printf("CHILD START TIME %ld \n", shm_ptr->tv_nsec);

        munmap(shm_ptr, obj_size);
        close(shmfd);

        execl(commandPath, commandName, NULL);
    }

    return 0;
}