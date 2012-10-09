#ifndef CONSOLE_TYPES_H
#define CONSOLE_TYPES_H

#include <app.h>
#include <glib.h>

typedef struct {
    app_t* app;
    GHashTable* commands;
    gchar* buffer;
    gboolean terminate;
} console_t;

typedef void (*console_cmd_ptr)(console_t*, gchar**);

#endif
