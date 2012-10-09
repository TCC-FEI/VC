#ifndef CONTROL_PLUGIN_H
#define CONTROL_PLUGIN_H

#include <glib.h>
#include <X11/Xlib.h>
#include <libwnck/libwnck.h>

typedef struct {
    Display* display;
    WnckScreen* screen;
} vcwm_t;

typedef struct {
    vcwm_t vcwm;
    GHashTable* commands;
} control_plugin_t;

typedef void (*control_command_t)(Display*);

void register_commands(control_plugin_t*);
void raise_window(control_plugin_t*);

#endif
