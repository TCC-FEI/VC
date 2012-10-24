#ifndef VC_CONSOLE_TYPES_H
#define VC_CONSOLE_TYPES_H

#include <vc/app.h>
#include <glib.h>

typedef struct {
    app_t* app;
    GHashTable* commands;
    gchar* buffer;
    gboolean terminate;
} console_t;

typedef void (*console_cmd_ptr)(console_t*, gchar**);

#endif
