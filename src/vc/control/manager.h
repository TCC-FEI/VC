#ifndef VC_CONTROL_MANAGER_H
#define VC_CONTROL_MANAGER_H

#include <vc/vc.h>
#include <vc/control/plugin.h>

#include <glib.h>
#include <X11/Xlib.h>
#include <libwnck/libwnck.h>

typedef gboolean (*execute_t)(gpointer, gchar*);

typedef struct {
    vcwm_t vcwm;
    GHashTable* plugins;
    gchar* plugin_dir;
    execute_t execute;
} control_manager_t;

gpointer control_manager_create(vcwm_t);
void control_manager_destroy(gpointer);

gboolean control_manager_load(gpointer, gchar*);
gboolean control_manager_unload(gpointer, gchar*);
gboolean control_manager_execute(gpointer, gchar*);

#endif
