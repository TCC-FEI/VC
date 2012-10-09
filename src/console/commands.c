#include <console/commands.h>
#include <stdio.h>

void _cc_help_print(gpointer key, gpointer value, gpointer data);

void cc_help(console_t* self, gchar** args) {
    printf("Commands:\n");

    g_hash_table_foreach(self->commands, _cc_help_print, NULL);
}

void cc_exit(console_t* self, gchar** args) {
    (void) args;

    self->terminate = TRUE;
}

void cc_exec(console_t* self, gchar** args) {

}

void _cc_help_print(gpointer key, gpointer value, gpointer data) {
    printf("\t%s: [%p]\n", (char*) key, value);
}
