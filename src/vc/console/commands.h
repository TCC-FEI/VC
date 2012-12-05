#ifndef VC_CONSOLE_COMMANDS_H
#define VC_CONSOLE_COMMANDS_H

#include <vc/vc.h>
#include <vc/console/types.h>

#include <glib.h>

void cc_help(console_t*, gchar**);
void cc_exit(console_t*, gchar**);
void cc_exec(console_t*, gchar**);
void cc_ctrl(console_t*, gchar**);
void cc_spch(console_t*, gchar**);

#endif
