#include <control/handler.h>

#include <stdlib.h>

control_handler_t* handler_create() {
    return (control_handler_t*) malloc(sizeof(control_handler_t));
}

void handler_destroy(gpointer handler) {
    if (handler) {
        free(handler);
    }
}
