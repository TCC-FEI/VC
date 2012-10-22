#ifndef VC_SPEECH_PLUGIN_H
#define VC_SPEECH_PLUGIN_H

#include <config.h>
#include <glib.h>
#include <vc/control/manager.h>

typedef struct speech_plugin_st speech_plugin_t;

gpointer create(gpointer);
void destroy(gpointer);

gboolean start(gpointer);
gboolean stop(gpointer);

#endif
