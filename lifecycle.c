#include <pthread.h>
#include "errors.h"

void * run_me(void *arg) {
    printf("\nGot %d\n",*(int*)arg);
    return arg;
}

int main(int argc, char *argv[]){
    pthread_t tid;
    int a = 5;
    int val=0;
    void *retval;
    retval = &val;
    pthread_create(&tid, NULL, run_me, &a);
    pthread_join(tid, &retval);
    printf("\nExited with %d\n", val);
}
