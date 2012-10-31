#include <vc/control/plugin.h>

#include <stdio.h>

void up(gpointer data);
void down(gpointer data);
void left(gpointer data);
void right(gpointer data);

guint32 register_commands(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return CTRL_INVALID_SELF;

    if (!self->commands)
        return CTRL_INVALID_COMMAND_TABLE;

    g_hash_table_insert(self->commands, "up", up);
    g_hash_table_insert(self->commands, "down", down);
    g_hash_table_insert(self->commands, "left", left);
    g_hash_table_insert(self->commands, "right", right);

    return CTRL_SUCCESS;
}

guint32 raise_window(gpointer data) {
    return CTRL_SUCCESS;
}

void up(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(1);

    key_set_add(set, XK_Up);

    send_key_event(display, set);

    key_set_del(set);
}

void down(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(1);

    key_set_add(set, XK_Down);

    send_key_event(display, set);

    key_set_del(set);
}

void left(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(1);

    key_set_add(set, XK_Left);

    send_key_event(display, set);

    key_set_del(set);
}

void right(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(1);

    key_set_add(set, XK_Right);

    send_key_event(display, set);

    key_set_del(set);
}
