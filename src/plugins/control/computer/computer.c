#include <vc/control/plugin.h>

#include <stdio.h>

void up(gpointer data);
void down(gpointer data);
void left(gpointer data);
void right(gpointer data);
void tab(gpointer data);
void enter(gpointer data);
void _close(gpointer data);

guint32 register_commands(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return CTRL_INVALID_SELF;

    if (!self->commands)
        return CTRL_INVALID_COMMAND_TABLE;

    g_hash_table_insert(self->commands, "up", up);
    g_hash_table_insert(self->commands, "rotate", up);
    g_hash_table_insert(self->commands, "down", down);
    g_hash_table_insert(self->commands, "left", left);
    g_hash_table_insert(self->commands, "back", left);
    g_hash_table_insert(self->commands, "right", right);
    g_hash_table_insert(self->commands, "forward", right);
    g_hash_table_insert(self->commands, "enter", enter);
    g_hash_table_insert(self->commands, "confirm", enter);
    g_hash_table_insert(self->commands, "tab", tab);
    g_hash_table_insert(self->commands, "close", _close);

    return CTRL_SUCCESS;
}

guint32 raise_window(gpointer data) {
    return CTRL_SUCCESS;
}

guint32 run_app(gpointer data) {
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

void tab(gpointer data){
    Display* display = (Display*) data;
    GArray* set = key_set_new(1);

    key_set_add(set, XK_Tab);

    send_key_event(display, set);

    key_set_del(set);
}

void enter(gpointer data){
    Display* display = (Display*) data;
    GArray* set = key_set_new(1);

    key_set_add(set, XK_Return);

    send_key_event(display, set);

    key_set_del(set);
}

void _close(gpointer data){
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Alt_L);
    key_set_add(set, XK_F4);

    send_key_event(display, set);

    key_set_del(set);
}
