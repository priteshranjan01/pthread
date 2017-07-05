#include <sys/types.h>
#include <sys/wait.h>
#include "errors.h"

int main() {
    char line[128];
    char message[64];
    int seconds;
    pid_t pid;

    while(1) {
        printf("\nALARM (duration message)> ");
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        if (strlen(line) < 1)  continue;

        if (sscanf(line, "%d %s[^\n]",&seconds, message) < 2) {
            printf("\nBad command");
            continue;
        }

        pid = fork();
        if (pid == (pid_t)-1) {
            errno_abort("Fork");
        } else if (pid == (pid_t)0) {
            // Child process
            sleep(seconds);
            printf("\n %d %s \n",seconds, message); 
            exit(0);
        } else {
            // Parent process
            do { // Collect dead children.
                pid = waitpid((pid_t)-1, NULL, WNOHANG);
                if (pid == (pid_t)-1)
                    errno_abort("Wait for child");
            } while(pid != (pid_t)0);
        }
    }
    return 0;
}
