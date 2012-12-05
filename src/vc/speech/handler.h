#ifndef VC_SPEECH_HANDLER_H
#define VC_SPEECH_HANDLER_H

#include <vc/vc.h>
#include <vc/speech/plugin.h>

typedef speech_plugin_t* (*create_t)();
typedef void (*destroy_t)(speech_plugin_t*);
typedef gboolean (*start_t)(speech_plugin_t*);
typedef gboolean (*stop_t)(speech_plugin_t*);

typedef struct {
    void* lib;
    speech_plugin_t* instance;
    create_t create;
    destroy_t destroy;
    start_t start;
    stop_t stop;
} speech_handler_t;

speech_handler_t* speech_handler_create(gchar*);
void speech_handler_destroy(gpointer);

#endif
