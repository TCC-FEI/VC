#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H

#include <vc.h>
#include <console/types.h>

#include <glib.h>

void cc_help(console_t*, gchar**);
void cc_exit(console_t*, gchar**);
void cc_exec(console_t*, gchar**);
void cc_ctrl(console_t*, gchar**);
void cc_spch(console_t*, gchar**);

#endif
