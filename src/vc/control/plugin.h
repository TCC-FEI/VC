#ifndef VC_CONTROL_PLUGIN_H
#define VC_CONTROL_PLUGIN_H

#include <config.h>

#include <glib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
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

GArray* key_set_new(guint);
void key_set_add(GArray*, KeySym);
void key_set_del(GArray*);
void send_key_event(Display*, GArray*);

#endif
