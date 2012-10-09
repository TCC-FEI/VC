#include <control/plugin.h>
#include <control/plugin_tools.h>

#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

control_plugin_t* create(vcwm_t);
void destroy(control_plugin_t*);
GHashTable* commands(control_plugin_t*);

control_plugin_t* create(vcwm_t vcwm) {
    control_plugin_t* plugin = NULL;

    plugin = (control_plugin_t*) malloc(sizeof(control_plugin_t));

    if (plugin == NULL) {
        return NULL;
    }

    plugin->commands = g_hash_table_new(g_str_hash, g_str_equal);

    if (plugin->commands == NULL) {
        free(plugin);
        return NULL;
    }

    plugin->vcwm = vcwm;

    register_commands(plugin);

    return plugin;
}

void destroy(control_plugin_t* plugin) {
    if (plugin) {
        if (plugin->commands) {
            g_hash_table_destroy(plugin->commands);
        }

        free(plugin);
    }
}

GHashTable* commands(control_plugin_t* plugin) {
    return plugin->commands;
}

GArray* key_set_new(guint len) {
    return g_array_new(FALSE, FALSE, sizeof(KeySym));
}

void key_set_add(GArray* set, KeySym key) {
    g_array_append_val(set, key);
}

void key_set_del(GArray* set) {
    g_array_free(set, TRUE);
}

void send_key_event(Display* display, GArray* keys) {
    int i;
    KeySym sym;
    KeyCode code;

    for (i = 0; i < keys->len; i++) {
        sym = g_array_index(keys, KeySym, i);
        code = XKeysymToKeycode(display, sym);
        XTestFakeKeyEvent(display, code, True, 0);
    }

    for (i = 0; i < keys->len; i++) {
        sym = g_array_index(keys, KeySym, i);
        code = XKeysymToKeycode(display, sym);
        XTestFakeKeyEvent(display, code, False, 0);
    }

    XFlush(display);
}
