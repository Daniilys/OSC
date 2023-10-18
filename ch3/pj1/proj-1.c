#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_ARGS 10

void strip_line(char **line) {
    char c = (*line)[0];
    while (isspace(c) || c == '\n') {
        (*line)++;
        c = (*line)[0];
    }
}

void process_args() {

}

int main(void) {

    int should_run = 1;

    char line_h[MAX_LINE];

    while (should_run && should_run < 10) {
        should_run++;

        printf("osh>");
        fflush(stdout);

        char line[MAX_LINE];
        char *orig_ptr = &line[0];

        fgets(line, MAX_LINE, stdin);
        strip_line(&orig_ptr);

        line[strlen(line) - 1] = '\0'; /* delete line-feed */

        if (strcmp("exit", line) == 0) {
            should_run = 0;
            break;
        }

        if (strcmp("!!", line) == 0) {

            if (strlen(line_h) == 0) {
                printf("No last command");
                fflush(stdout);
                continue;
            }
            strcpy(line, line_h);
        } else {
            strcpy(line_h, line);
        }

        char *args[MAX_ARGS];

        char *token = strtok(line, " "); // Tokenize the input string using space as the delimiter
        int count = 0;
        for (; token != NULL && count < MAX_ARGS; count++) {
            args[count] = token;
            token = strtok(NULL, " ");
        }

        int wait_flag = 0;

        if (strcmp(args[count - 1], "&") == 0) {
            wait_flag = 1;
            args[count - 1] = NULL;
        }

        pid_t pid = fork();

        if (pid == 0) {
            should_run = 0;
            execvp(args[0], args);
        } else {

            if (wait_flag) {
                int status;
                wait(&status);
            }
        }
    }

    return 0;
}

