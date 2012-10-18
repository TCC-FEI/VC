#ifndef SPEECH_MANAGER_H
#define SPEECH_MANAGER_H

#include <glib.h>
#include <speech/handler.h>

typedef struct {
    int* argc;
    char*** argv;
    speech_handler_t* handler;
    gchar* plugin_dir;
} speech_manager_t;

speech_manager_t* speech_manager_create(int*, char***);
void speech_manager_destroy(speech_manager_t*);

gboolean speech_manager_load(speech_manager_t*, gchar*);
gboolean speech_manager_unload(speech_manager_t*);

#endif
