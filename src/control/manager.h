#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <glib.h>
#include <X11/Xlib.h>
#include <libwnck/libwnck.h>
#include <control/plugin.h>

typedef struct {
    vcwm_t vcwm;
    GHashTable* plugins;
    gchar* plugin_dir;
} control_manager_t;

control_manager_t* control_manager_create(vcwm_t);
void control_manager_destroy(control_manager_t*);

gboolean control_manager_load(control_manager_t*, gchar*);
gboolean control_manager_unload(control_manager_t*, gchar*);
gboolean control_manager_execute(control_manager_t*, gchar*);

#endif
