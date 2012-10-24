#include <vc/control/plugin.h>

#include <stdio.h>

void file_new(gpointer data);
void file_close(gpointer data);
void file_quit(gpointer data);

guint32 register_commands(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return CTRL_INVALID_SELF;

    if (!self->commands)
        return CTRL_INVALID_COMMAND_TABLE;

    g_hash_table_insert(self->commands, "file new", file_new);
    g_hash_table_insert(self->commands, "file close", file_close);
    g_hash_table_insert(self->commands, "file quit", file_quit);

    return CTRL_SUCCESS;
}

guint32 raise_window(gpointer data) {
    guint32 status;

    GList* list;
    WnckWindow* window;
    WnckApplication* app;

    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return CTRL_INVALID_SELF;

    if (!self->vcwm.screen)
        return CTRL_INVALID_SCREEN;

    status = CTRL_WINDOW_NOT_FOUND;
    list = wnck_screen_get_windows(self->vcwm.screen);

    for (list; list != NULL; list = list->next) {
        window = WNCK_WINDOW(list->data);

        if (!window)
            continue;

        app = wnck_window_get_application(window);

        if (!app)
            continue;

        if (strcmp(wnck_application_get_name(app), "gedit") == 0) {
            wnck_window_activate(window, (guint32) time(NULL));
            status = CTRL_SUCCESS;
            break;
        }
    }

    return status;
}

void file_new(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_N);

    send_key_event(display, set);

    key_set_del(set);
}

void file_close(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_W);

    send_key_event(display, set);

    key_set_del(set);
}

void file_quit(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_Q);

    send_key_event(display, set);

    key_set_del(set);
}
