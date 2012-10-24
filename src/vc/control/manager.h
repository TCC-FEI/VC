#ifndef VC_CONTROL_MANAGER_H
#define VC_CONTROL_MANAGER_H

#include <vc/vc.h>
#include <vc/control/plugin.h>

#include <glib.h>
#include <X11/Xlib.h>
#include <libwnck/libwnck.h>

#define CTRL_PLUGIN_DEFAULT "computer"

typedef gboolean (*execute_t)(gpointer, gchar*);

typedef struct {
    vcwm_t vcwm;
    gchar* plugin_dir;
    GHashTable* plugins;
    execute_t execute;
} control_manager_t;

typedef struct {
    vcwm_t vcwm;
    char* plugin_dir;
} control_manager_args_t;

gpointer control_manager_create(gpointer);
void control_manager_destroy(gpointer);
void control_manager_list(gpointer);

gboolean control_manager_load(gpointer, gchar*);
gboolean control_manager_unload(gpointer, gchar*);
gboolean control_manager_execute(gpointer, gchar*);

#endif
