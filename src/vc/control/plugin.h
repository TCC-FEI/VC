#ifndef VC_CONTROL_PLUGIN_H
#define VC_CONTROL_PLUGIN_H

#include <config.h>

#include <glib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <libwnck/libwnck.h>

#define CTRL_SUCCESS                   0
#define CTRL_INVALID_SELF              1
#define CTRL_INVALID_SCREEN            2
#define CTRL_INVALID_DISPLAY           4
#define CTRL_INVALID_COMMAND_TABLE     8
#define CTRL_INVALID_KEYS             16
#define CTRL_WINDOW_NOT_FOUND         32

typedef struct {
    Display* display;
    WnckScreen* screen;
} vcwm_t;

typedef struct {
    vcwm_t vcwm;
    GHashTable* commands;
} control_plugin_t;

guint32 register_commands(gpointer);
guint32 run_app(gpointer);
guint32 raise_window(gpointer);

GArray* key_set_new(guint);
guint32 key_set_add(GArray*, KeySym);
guint32 key_set_del(GArray*);
guint32 send_key_event(Display*, GArray*);

#endif
