#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    char *origin[] = {"one", "two"};
    size_t itemsCount = sizeof(origin) / sizeof(char *);

    char **source = malloc((itemsCount + 1) * sizeof(char *));

    for (size_t i = 0; i < itemsCount; ++i) {
        printf("%s \n", *(origin + i));
        source[i] = origin[i];
    }

    source[itemsCount] = NULL;

    for (size_t i = 0; source[i] != NULL; ++i) {
        printf("%s \n", *(source + i));
    }

    return 0;
}