#include <pthread.h>
#include "errors.h"

int main(int argc, char *argv[])
{
    int seconds;
    char line[128];
    char message[64];

    while(1) {
        printf("\nAlarm :");
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        printf("\n%s\n",line);
        if (strlen(line) <= 1) continue;

        if (sscanf (line, "%2d %64s",
                    &seconds, message) < 2) {
            fprintf(stderr, "Bad command\n");
        } else {
            sleep(seconds);
            printf("(%d) %s\n", seconds, message);
        }
    }
}
