#include "errors.h"
#include <pthread.h>
#include <time.h>

typedef struct alarm_tag {
    struct alarm_tag *next;
    int time;
    int seconds;
    char message[64];
}alarm_t;

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;

alarm_t  *alarm_list = NULL;

void *alarm_thread(void * ab) {
    alarm_t *temp;
    int status;
    unsigned int now, sleep_time;
    while(1) {
        status = pthread_mutex_lock(&alarm_mutex);
        if (status != 0) {
            err_abort(status, "Thread mutex lock error");
        }
        temp = alarm_list;
        if (temp == NULL) {
            sleep_time = 1;
        } else {
            now = time(NULL);
            if (temp->time <= now) {
                sleep_time = 0;
            } else {
                sleep_time =  temp->time - now;
            }
            alarm_list = temp->next;
        }
        status = pthread_mutex_unlock(&alarm_mutex);
        if (status != 0) {
            err_abort(status, "Thread mutex UNlock error");
        }
        if (sleep_time > 0) {
            sleep(sleep_time);
        } else {
            sched_yield();
        }
        if (temp != NULL) {
            printf(" %d %s \n", temp->seconds, temp->message);
            free(temp);
        }
    }
}

int main() {

    int seconds;
    char line[128];
    char message[64];
    alarm_t *alarm_node, **last, *next;
    pthread_t tid;
    int status;
    while(1) {
        // TODO: Create and run the mutex
        pthread_create(&tid, NULL, alarm_thread, NULL);
        printf("\nAlarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) 
            errno_abort("Failed to accept input");
        if (sscanf(line, "%d %s",&seconds, message) < 2) {
            printf("\n Bad format %s sample format> 23 wake_up",line);
        printf("\n[List: ");
        for(next = alarm_list; next != NULL; next=next->next) {
            printf ("%d %d(%ld)[\"%s\"] ",next->seconds, next->time, next->time - time(NULL), next->message);
        }
            continue;
        }
        alarm_node = (alarm_t *)malloc(sizeof(alarm_t));
        if (alarm_node == NULL) {
            errno_abort("Failed to allocate memory");
        }
        status = pthread_mutex_lock(&alarm_mutex);
        if (status != 0) {
            err_abort(status, "Pthread lock");
        }
        printf("\nLocked");
        // Node created.
        alarm_node->seconds = seconds;
        strncpy(alarm_node->message, message, (size_t)64);
        alarm_node->time = time(NULL) + seconds;
        alarm_node->next = NULL;

        last = &alarm_list;
        next = *last;

        while (next != NULL) {
            if (next->time >= alarm_node->time) {
                alarm_node->next = next;
                *last = alarm_node;
                break;
            }
            last = &next->next;
            next = next->next;
        }
        if (next == NULL) {
            *last = alarm_node;
            alarm_node->next = NULL;
        }
        printf("] \n");

        status = pthread_mutex_unlock(&alarm_mutex);
        if (status != 0) {
            err_abort(status, "unlock mutex");
        }
        printf("\nUnlocked");
    }
    return 1;
}
