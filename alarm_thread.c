#include <pthread.h>
#include "errors.h"

typedef struct alarm_tag {
    int seconds;
    char * message;
}alarm_t;

void *alarm_thread (void *arg) {
    alarm_t *alarm = (alarm_t *)arg;
    int status;

    if (alarm == NULL) {
        errno_abort("Arg was NULL");
    }

    status =  pthread_detach(pthread_self());

    if (status != 0)
        err_abort(status, "Thread detach failed");
    //printf("\nAlarm for %d seconds, %s\n",alarm->seconds, alarm->message);
    sleep(alarm->seconds);
    printf("\n(%d)seconds:  %s\n",alarm->seconds, alarm->message);
    free(alarm->message);
    free(alarm);
    pthread_exit(0);
}

int main(){
    char line[128];
    char message[64];
    int seconds;
    alarm_t *alarm = NULL;
    pthread_t thread;
    int status;

    while(1)
    {
        printf("\nAlarm seconds message:> ");
        if(fgets(line, sizeof(line), stdin) == NULL) exit(0);
        if (sscanf(line, "%d %s[^\n]",&seconds, message) < 2) 
            printf("\nBad command");
        else {
            alarm = (alarm_t *)malloc(sizeof(alarm_t));
            alarm->message = NULL;
            alarm->seconds = seconds;
            alarm->message = strdup(message);
            if (alarm == NULL)
                err_abort(errno, "Malloc failure");
            status = pthread_create(&thread, NULL, alarm_thread, alarm);
            if (status != 0){
                free(alarm->message);
                free(alarm);
                err_abort(status, "Pthread Failed");
            }
        }
    }
}
