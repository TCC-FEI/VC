#include <vc/control/plugin.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

gpointer create( gpointer);
void destroy( gpointer);
GHashTable* commands( gpointer);

gpointer create(gpointer data) {
    control_plugin_t* self = NULL;

    self = g_try_new0(control_plugin_t, 1);
    if (!self)
        return NULL;

    self->commands = g_hash_table_new(g_str_hash, g_str_equal);

    if (!self->commands) {
        destroy(self);
        return NULL;
    }

    self->vcwm = *((vcwm_t*) data);

    return self;
}

void destroy(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (self) {
        if (self->commands) {
            g_hash_table_destroy(self->commands);
        }

        g_free(self);
    }
}

GHashTable* commands(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return NULL;

    return self->commands;
}

GArray* key_set_new(guint len) {
    return g_array_sized_new(FALSE, FALSE, sizeof(KeySym), len);
}

guint32 key_set_add(GArray* keys, KeySym key) {
    if (keys && keys == g_array_append_val(keys, key))
        return CTRL_SUCCESS;
    else
        return CTRL_INVALID_KEYS;
}

guint32 key_set_del(GArray* keys) {
    if (keys && NULL == g_array_free(keys, TRUE))
        return CTRL_SUCCESS;
    else
        return CTRL_INVALID_KEYS;
}

guint32 send_key_event(Display* display, GArray* keys) {
    int i;
    KeySym sym;
    KeyCode code;

    if (!display)
        return CTRL_INVALID_DISPLAY;

    if (!keys)
        return CTRL_INVALID_KEYS;

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
