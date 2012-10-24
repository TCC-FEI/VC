#ifndef VC_UTILS_H
#define VC_UTILS_H

#include <glib.h>
#include <stdio.h>

void _vc_print_hash(gpointer, gpointer, gpointer);
void _vc_trace_hash(gchar*, GHashTable*);
void _vc_trace_strv(gchar* name, gchar** strv);

#endif
