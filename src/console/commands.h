#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H

#include <console/types.h>
#include <glib.h>

void cc_help(console_t* self, gchar** args);
void cc_exit(console_t* self, gchar** args);
void cc_exec(console_t* self, gchar** args);


#endif
