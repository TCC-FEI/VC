#ifndef VC_CONTROL_HANDLER_H
#define VC_CONTROL_HANDLER_H

#include <vc/vc.h>
#include <vc/control/plugin.h>

typedef control_plugin_t* (*create_t)(vcwm_t);
typedef void (*destroy_t)(control_plugin_t*);
typedef GHashTable* (*commands_t)(control_plugin_t*);
typedef void (*raise_t)(control_plugin_t*);

typedef struct {
    void* lib;
    control_plugin_t* instance;
    create_t create;
    destroy_t destroy;
    commands_t commands;
    raise_t raise;
} control_handler_t;

control_handler_t* handler_create(gchar*);
void handler_destroy(gpointer);

#endif
