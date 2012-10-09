#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <console/console.h>
#include <console/types.h>
#include <console/commands.h>

console_t* _initialize(gpointer data);
void _finalize(console_t* self);

void _read(console_t* self);
void _execute(console_t* self);

gpointer console(gpointer data) {
    console_t* self = _initialize(data);

    printf("console starting\n");

    while (!self->terminate) {
        printf("vc> ");
        _read(self);
        _execute(self);
    }

    printf("console ending\n");

    g_main_loop_quit(self->app->main_loop);
    g_main_loop_unref(self->app->main_loop);
    _finalize(self);

    return NULL;
}

console_t* _initialize(gpointer data) {
    console_t* self = (console_t*) malloc(sizeof(console_t));

    if (self) {
        self->app = (app_t*) data;
        self->terminate = FALSE;
        self->buffer = g_strnfill(MAX_BUFF_SIZE, '\0');
        self->commands = g_hash_table_new(g_str_hash, g_str_equal);

        g_hash_table_insert(self->commands, "help", cc_help);
        g_hash_table_insert(self->commands, "exit", cc_exit);
        g_hash_table_insert(self->commands, "exec", cc_exec);
    }

    return self;
}

void _finalize(console_t* self) {
    if (self) {
        if (self->buffer) {
            g_free(self->buffer);
        }

        if (self->commands) {
            g_hash_table_destroy(self->commands);
        }

        free(self);
    }
}

void _read(console_t* self) {
    if (fgets(self->buffer, MAX_BUFF_SIZE, stdin)) {
        self->buffer[strlen(self->buffer) - 1] = '\0';
    }
}

void _execute(console_t* self) {
    if (strlen(self->buffer) > 0) {
        gchar** args = g_strsplit(self->buffer, " ", 0);

        console_cmd_ptr cmd = g_hash_table_lookup(self->commands, args[0]);

        if (cmd != NULL) {
            (*cmd)(self, args);
        }

        g_strfreev(args);
    }
}
