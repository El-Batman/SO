#include "module.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

pthread_t thread;
int cancelled;
int setup = 0;

static void* worker(void* ignored) {
    while (cancelled == 0) {
        rock_t rock = pd_read();
        oil_t oil = pd_process(rock);
        pd_deliver(oil);
    }
    return NULL;
}

void mod_setup() {
    assert(setup == 0);
    setup = 1;
    cancelled = 0;
    for (int i = 0; i <= PROCESSORS_COUNT; i++){
      pthread_create(&thread, NULL, &worker, NULL);
      pthread_cond_wait();
    }
}

char* mod_name() {
    return "Wayne Corp Oil";
}

void mod_shutdown() {
    assert(setup == 1);
    cancelled = 1;
    pthread_join(thread, NULL);
}
