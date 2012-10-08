#include "console.h"
#include "app.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    app_t* app;
    GHashTable* commands;
    gchar* buffer;
    gboolean terminate;
} console_t;

typedef void (*console_cmd_ptr)(console_t*, gchar**);

console_t* _initialize(gpointer data);
void _finalize(console_t* self);

gchar* _read(console_t* self);
void _execute(console_t* self, gchar* buffer);

void _cmd_exit(console_t* self, gchar** args);
void _cmd_help(console_t* self, gchar** args);
void _cmd_help_print( gpointer key, gpointer value, gpointer data );

gpointer console(gpointer data) {
    gchar* cmd;

    console_t* self = _initialize(data);

    printf("console starting\n");

    while (!self->terminate) {
        printf("vc> ");
        cmd = _read(self);
        _execute(self, cmd);
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

        g_hash_table_insert(self->commands, "exit", _cmd_exit);
        g_hash_table_insert(self->commands, "help", _cmd_help);
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

gchar* _read(console_t* self) {
    if (fgets(self->buffer, MAX_BUFF_SIZE, stdin) != 0) {
        self->buffer[strlen(self->buffer) - 1] = '\0';
    }

    return self->buffer;
}

void _execute(console_t* self, gchar* buffer) {
    if (strlen(buffer) > 0) {
        gchar** args = g_strsplit(buffer, " ", 0);

        console_cmd_ptr cmd = g_hash_table_lookup(self->commands, args[0]);

        if (cmd != NULL) {
            (*cmd)(self, args);
        }
    }
}

void _cmd_exit(console_t* self, gchar** args) {
    self->terminate = TRUE;
}

void _cmd_help(console_t* self, gchar** args) {
    printf("Commands:\n");

    g_hash_table_foreach( self->commands, _cmd_help_print, NULL );
}

void _cmd_help_print( gpointer key, gpointer value, gpointer data ) {
    printf("\t%s: [%p]\n", (char*)key, value );
}
