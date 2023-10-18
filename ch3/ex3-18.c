#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  
  pid_t pid;
  
  pid = fork();

  if (pid > 0) {
    printf("child pid: %d \n", pid);
    printf("my pid is %d \n", getpid());

    while (1)
    {
      /* code */
    }
    
  }


  return 0; 
}