#include "errors.h"
#include <pthread.h>

struct my_struct_tag {
    pthread_mutex_t mut;
    int val;
};

typedef struct my_struct_tag my_struct_t;


int main() {

    my_struct_t data;
    int status;
    status = pthread_mutex_init(&data.mut, NULL);
    if (status != 0) {
        err_abort(status, "Mutex init");
    } else {
        printf("Mutex initialized");
    }
    status = pthread_mutex_destroy(&data.mut);
    if (status != 0) {
        err_abort(status, "Mutex destroy");
    }
    return 0;
}
