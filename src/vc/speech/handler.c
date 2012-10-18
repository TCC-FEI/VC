#include <vc/speech/handler.h>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

speech_handler_t* speech_handler_create(gchar* filename) {
    speech_handler_t* self;
    char* error;

    self = (speech_handler_t*) malloc(sizeof(speech_handler_t));
    if (!self) {
        vc_trace("Falha ao alocar memória para vc::speech::handler\n");
        return NULL;
    }

    self->lib = dlopen(filename, RTLD_LAZY);
    vc_trace("Carregando biblioteca dinâmica em [%p]\n", self->lib);
    if (!self->lib) {
        vc_trace("%s\n", dlerror());
        speech_handler_destroy(self);
        return NULL;
    }

    dlerror();
    *(void **) (&(self->create)) = dlsym(self->lib, "create");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        speech_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'create' em:       [%p]\n", self->create);

    dlerror();
    *(void **) (&(self->destroy)) = dlsym(self->lib, "destroy");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        speech_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'destroy' em:      [%p]\n", self->destroy);

    dlerror();
    *(void **) (&(self->start)) = dlsym(self->lib, "start");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        speech_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'start' em: [%p]\n", self->start);

    dlerror();
    *(void **) (&(self->stop)) = dlsym(self->lib, "stop");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        speech_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'stop' em: [%p]\n", self->stop);

    return self;
}

void speech_handler_destroy(gpointer handler) {
    speech_handler_t* self = (speech_handler_t*) handler;

    if (self) {
        if (self->instance) {
            vc_trace("Liberando memória da instância [%p]\n", self->instance);
            if (self->destroy) {
                vc_trace("Invocando 'destroy' [%p]\n", self->destroy);
                (*(self->destroy))(self->instance);
            } else {
                vc_trace("Forçando com 'free()'\n");
                free(self->instance);
            }
        }

        if (self->lib) {
            vc_trace("Descarregando biblioteca dinâmmica [%p]\n", self->lib);
        }

        vc_trace("Liberando memória do speech::handler [%p]\n", self);
        free(self);
    }
}
