#include "errors.h"
#include <pthread.h>

struct my_struct_tag {
    pthread_mutex_t mutex;
    int val;
};

struct my_struct_tag  data = {PTHREAD_MUTEX_INITIALIZER, 0};

int main(){
    return 1;
}
