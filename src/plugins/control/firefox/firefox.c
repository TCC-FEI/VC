#include <vc/control/plugin.h>

#include <stdio.h>

void new_tab(gpointer data);
void print(gpointer data);
void quit(gpointer data);
void select_all(gpointer data);
void cut(gpointer data);
void copy(gpointer data);
void paste(gpointer data);
void save(gpointer data);
void focus(gpointer data);
void bookmarks(gpointer data);

guint32 register_commands(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return CTRL_INVALID_SELF;

    if (!self->commands)
        return CTRL_INVALID_COMMAND_TABLE;

    g_hash_table_insert(self->commands, "run", run_app);
    g_hash_table_insert(self->commands, "new tab", new_tab);
    g_hash_table_insert(self->commands, "print", print);
    g_hash_table_insert(self->commands, "quit", quit);
    g_hash_table_insert(self->commands, "select all", select_all);
    g_hash_table_insert(self->commands, "cut", cut);
    g_hash_table_insert(self->commands, "copy", copy);
    g_hash_table_insert(self->commands, "paste", paste);
    g_hash_table_insert(self->commands, "save", save);
    g_hash_table_insert(self->commands, "focus", focus);
    g_hash_table_insert(self->commands, "bookmarks", bookmarks);
    g_hash_table_insert(self->commands, "close bookmarks", bookmarks);

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

        if (strcmp(wnck_application_get_name(app), "Firefox") == 0) {
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
    g_shell_parse_argv("/usr/bin/firefox", &argc, &argv, NULL);
    g_spawn_async(NULL, argv, NULL,
        G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_STDERR_TO_DEV_NULL, NULL, NULL,
        NULL, NULL);
    g_strfreev(argv);
}

void new_tab(gpointer data) {
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

void quit(gpointer data) {
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

void focus(gpointer data){

}

void bookmarks(gpointer data){
    Display* display = (Display*) data;
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_B);

    send_key_event(display, set);

    key_set_del(set);
}
