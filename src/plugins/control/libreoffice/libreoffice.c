#include <vc/control/plugin.h>

#include <stdio.h>

void new_presentation(gpointer data);
void print(gpointer data);
void exit(gpointer data);
void select_all(gpointer data);
void cut(gpointer data);
void copy(gpointer data);
void paste(gpointer data);
void save(gpointer data);

guint32 register_commands(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return CTRL_INVALID_SELF;

    if (!self->commands)
        return CTRL_INVALID_COMMAND_TABLE;

    g_hash_table_insert(self->commands, "run", run_app);
    g_hash_table_insert(self->commands, "new presentation", new_presentation);
    g_hash_table_insert(self->commands, "print", print);
    g_hash_table_insert(self->commands, "exit", exit);
    g_hash_table_insert(self->commands, "select all", select_all);
    g_hash_table_insert(self->commands, "cut", cut);
    g_hash_table_insert(self->commands, "copy", copy);
    g_hash_table_insert(self->commands, "paste", paste);
    g_hash_table_insert(self->commands, "save", save);

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

        if (g_str_has_prefix(wnck_application_get_name(app), "Libreoffice")) {
            wnck_window_activate(window, (guint32) time(NULL));
            status = CTRL_SUCCESS;
            break;
        }
    }

    return status;
}

guint32 run_app(gpointer data) {
    gint argc;
    gchar** argv;
    g_shell_parse_argv("/usr/bin/libreoffice", &argc, &argv, NULL);
    g_spawn_async(NULL, argv, NULL,
        G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_STDERR_TO_DEV_NULL, NULL, NULL,
        NULL, NULL);
    g_strfreev(argv);
}

void new_presentation(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_N);

    send_key_event(display, set);

    key_set_del(set);
}

void print(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_P);

    send_key_event(display, set);

    key_set_del(set);
}

void exit(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_Q);

    send_key_event(display, set);

    key_set_del(set);
}

void select_all(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_A);

    send_key_event(display, set);

    key_set_del(set);
}

void cut(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_X);

    send_key_event(display, set);

    key_set_del(set);
}

void copy(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_C);

    send_key_event(display, set);

    key_set_del(set);
}

void paste(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_V);

    send_key_event(display, set);

    key_set_del(set);
}

void save(gpointer data) {
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_S);

    send_key_event(display, set);

    key_set_del(set);
}
