#ifndef VC_SPEECH_MANAGER_H
#define VC_SPEECH_MANAGER_H

#include <vc/vc.h>
#include <vc/speech/handler.h>
#include <vc/control/manager.h>

#include <glib.h>

typedef struct {
    speech_handler_t* handler;
    gchar* plugin_dir;
    control_manager_t* control;
} speech_manager_t;

speech_manager_t* speech_manager_create(gpointer);
void speech_manager_destroy(speech_manager_t*);

gboolean speech_manager_load(speech_manager_t*, gchar*);
gboolean speech_manager_unload(speech_manager_t*);

#endif
