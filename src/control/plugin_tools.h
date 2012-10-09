#ifndef CONTROL_PLUGIN_TOOLS_H
#define CONTROL_PLUGIN_TOOLS_H

#include <glib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

GArray* key_set_new(guint);
void key_set_add(GArray*, KeySym);
void key_set_del(GArray*);
void send_key_event(Display*, GArray*);

#endif
