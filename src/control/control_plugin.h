#ifndef CONTROL_PLUGIN_H
#define CONTROL_PLUGIN_H

#include <glib.h>
#include <X11/Xlib.h>

typedef struct {
    Display*        display;
    GHashTable*     commands;
} control_plugin_t;

typedef int (*cmd_func_t)(Display*);

control_plugin_t*   create_plugin(Display* display);
void                register_commands(control_plugin_t* plugin);
void                destroy_plugin(control_plugin_t* plugin);

#endif
