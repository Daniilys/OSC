#define MIN_PID 300
#define MAX_PID 5000
//Creates and initializes a data structure for representing pids; returns −1 if unsuccessful, 1 if successful 
int allocate_map(void);

//Allocates and returns a pid; returns −1 if unable to allocate a pid (all pids are in use)
int allocate_pid(void);

//Releases a pid
void release_pid(int pid);