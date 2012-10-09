#include <control/plugin.h>
#include <control/plugin_tools.h>

#include <stdlib.h>
#include <stdio.h>

void file_new(Display* display);
void file_close(Display* display);

void register_commands(control_plugin_t* plugin) {
    g_hash_table_insert(plugin->commands, "file new", file_new);
    g_hash_table_insert(plugin->commands, "file close", file_close);
}

void raise_window(control_plugin_t* plugin) {
    GList* list;
    WnckWindow* window;
    WnckApplication* app;

    list = wnck_screen_get_windows(plugin->vcwm.screen);

    for (list; list != NULL; list = list->next) {
        window = WNCK_WINDOW(list->data);
        app = wnck_window_get_application(window);
        if (strcmp(wnck_application_get_name(app), "gedit") == 0) {
            wnck_window_activate(window, (guint32) time(NULL));
            break;
        }
    }
}

void file_new(Display* display) {
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_N);

    send_key_event(display, set);

    key_set_del(set);
}

void file_close(Display* display) {
    GArray* set = key_set_new(2);

    key_set_add(set, XK_Control_L);
    key_set_add(set, XK_W);

    send_key_event(display, set);

    key_set_del(set);
}
