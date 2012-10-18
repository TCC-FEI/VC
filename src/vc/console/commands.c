#include <vc/console/commands.h>
#include <stdio.h>

void _cc_help_print(gpointer key, gpointer value, gpointer data);

void cc_help(console_t* self, gchar** args) {
    vc_trace_strv("console::commands::help() args", args);

    printf("Comandos:\n");
    g_hash_table_foreach(self->commands, _cc_help_print, NULL);
}

void cc_exit(console_t* self, gchar** args) {
    vc_trace_strv("console::commands::exit() args:", args);

    (void) args;
    self->terminate = TRUE;
}

void cc_ctrl(console_t* self, gchar** args) {
    gchar* command;
    control_manager_t* manager = self->app->control;

    vc_trace_strv("console::commands::ctrl() args:", args);

    if (g_strv_length(args) >= 2) {
        if (strcmp(args[1], "load") == 0) {
            control_manager_load(manager, args[2]);
        } else if (strcmp(args[1], "unload") == 0) {
            control_manager_unload(manager, args[2]);
        } else if (strcmp(args[1], "exec") == 0) {
            command = g_strjoinv(" ", &args[2]);
            control_manager_execute(manager, command);
            g_free(command);
        } else if (strcmp(args[1], "list") == 0) {
            control_manager_list(manager);
        } else {
            printf("comando '%s' não encontrado\n", args[1]);
        }
    } else {
        printf("comando inválido\n");
    }
}

void cc_spch(console_t* self, gchar** args) {
    gchar* command;
    speech_manager_t* manager = self->app->speech;

    vc_trace_strv("console::commands::spch() args:", args);

    if (g_strv_length(args) >= 2) {
        if (strcmp(args[1], "load") == 0) {
            speech_manager_load(manager, args[2]);
        } else if (strcmp(args[1], "unload") == 0) {
            speech_manager_unload(manager);
        } else if (strcmp(args[1], "start") == 0) {
            (*(manager->handler->start))(manager->handler->instance);
        } else if (strcmp(args[1], "stop") == 0) {
            (*(manager->handler->stop))(manager->handler->instance);
        } else {
            printf("comando '%s' não encontrado\n", args[1]);
        }
    } else {
        printf("comando inválido\n");
    }
}

void _cc_help_print(gpointer key, gpointer value, gpointer data) {
    printf("\t%s: [%p]\n", (char*) key, value);
}
