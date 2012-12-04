#include <vc/control/plugin.h>

#include <stdio.h>

void focus(gpointer data);

guint32 register_commands(gpointer data) {
    control_plugin_t* self = (control_plugin_t*) data;

    if (!self)
        return CTRL_INVALID_SELF;

    if (!self->commands)
        return CTRL_INVALID_COMMAND_TABLE;

    g_hash_table_insert(self->commands, "run", run_app);
    g_hash_table_insert(self->commands, "focus", focus);

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

        if (strcmp(wnck_application_get_name(app), "quadrapassel") == 0) {
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
    g_shell_parse_argv("/usr/games/quadrapassel", &argc, &argv, NULL);
    g_spawn_async(NULL, argv, NULL,
        G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_STDERR_TO_DEV_NULL, NULL, NULL,
        NULL, NULL);
    g_strfreev(argv);
}

void focus(gpointer data) {

}
