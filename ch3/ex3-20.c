#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ex3-20.h"

const size_t blockSizeBites = sizeof(u_int32_t) * 8;
size_t numberOfBlocks;
size_t lastFullBlock;
u_int32_t *ptr;

int allocate_map(void) {

    numberOfBlocks = (MAX_PID - MIN_PID) / (blockSizeBites);
    lastFullBlock = numberOfBlocks - 1;

    if (((MAX_PID - MIN_PID) % blockSizeBites) != 0) {
        ++numberOfBlocks;
    }

    ptr = malloc(numberOfBlocks * (blockSizeBites / 8));
    if (ptr == NULL) {
        return -1;
    }
    return 1;
}

u_int32_t is_pid_available(u_int32_t pid) {
    u_int32_t bit_idx = pid % blockSizeBites;
    u_int32_t idx = (pid - MIN_PID) / blockSizeBites;

    return !((ptr[idx] >> bit_idx) & 1);
}

void reserve_pid(u_int32_t pid) {
    u_int32_t bit_idx = pid % blockSizeBites;
    u_int32_t idx = (pid - MIN_PID) / blockSizeBites;
    ptr[idx] |= (1U << bit_idx);
}

int allocate_pid(void) {

    for (size_t n = 0; n < numberOfBlocks; ++n) {
        u_int32_t *item = &ptr[n];
        if (n <= lastFullBlock) {
            if (*item < ULONG_MAX) {
                for (int bitPosition = 0; bitPosition < blockSizeBites; bitPosition++) {
                    if ((*item & (1 << bitPosition)) == 0) {
                        *item |= (1 << bitPosition);
                        return n * blockSizeBites + bitPosition + MIN_PID;
                    }
                }
            }
        } else {
            printf("complete blocks end \n");
            for (int pid = MIN_PID + (lastFullBlock + 1) * blockSizeBites; pid <= MAX_PID; ++pid) {
                if (is_pid_available(pid)) {
                    reserve_pid(pid);
                    return pid;
                }
            }
        }
    }

    return -1;
}


void release_pid(int pid) {
    int internalId = pid - MIN_PID;
    int bitePosition = internalId % blockSizeBites;
    int itemPosition = internalId / blockSizeBites;

    *(&(ptr[itemPosition])) &= ~(1 << bitePosition);
}

int main() {
    if (allocate_map() == -1) {
        fprintf(stderr, "error allocate_map");
        return -1;
    }

    printf("allocate 10 \n");
    for (int n = 0; n < 64; ++n) {
        int pid = allocate_pid();
        printf("pid: %d \n", pid);
    }

    printf("release 5 \n");
    for (int n = 0; n < 5; ++n) {
        release_pid(MIN_PID + n);
        printf("release pid: %d \n", MIN_PID + n);
    }

    printf("allocate 4700 \n");
    for (int n = 0; n < 4700; ++n) {
        int pid = allocate_pid();
        printf("pid: %d \n", pid);
    }

    return 0;
}

