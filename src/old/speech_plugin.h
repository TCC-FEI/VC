#ifndef SPEECH_PLUGIN_H
#define SPEECH_PLUGIN_H


typedef struct {
    /** Dados aqui */
} speech_plugin_t;


speech_plugin_t* speech_plugin_create( const char* filename );
void speech_plugin_destroy( speech_plugin_t* plugin );

int speech_plugin_start( speech_plugin_t* plugin );
int speech_plugin_stop( speech_plugin_t* plugin );


#endif
