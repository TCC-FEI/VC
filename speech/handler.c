#include <speech/handler.h>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

speech_handler_t* handler_create(gchar* filename) {
    speech_handler_t* self;
    char* error;

    self = (speech_handler_t*) malloc(sizeof(speech_handler_t));
    if (!self) {
        printf("Falha ao alocar memória para speech::handler\n");
        return NULL;
    }

    self->lib = dlopen(filename, RTLD_LAZY);
    printf("Carregando biblioteca dinâmica em [%p]\n", self->lib);
    if (!self->lib) {
        printf("%s\n", dlerror());
        handler_destroy(self);
        return NULL;
    }

    dlerror();
    *(void **) (&(self->create)) = dlsym(self->lib, "create");
    if ((error = dlerror()) != NULL) {
        printf("%s\n", dlerror());
        handler_destroy(self);
        return NULL;
    }
    printf("Obtive 'create' em:       [%p]\n", self->create);

    dlerror();
    *(void **) (&(self->destroy)) = dlsym(self->lib, "destroy");
    if ((error = dlerror()) != NULL) {
        printf("%s\n", dlerror());
        handler_destroy(self);
        return NULL;
    }
    printf("Obtive 'destroy' em:      [%p]\n", self->destroy);

    dlerror();
    *(void **) (&(self->recognize)) = dlsym(self->lib, "recognize_command");
    if ((error = dlerror()) != NULL) {
        printf("%s\n", dlerror());
        handler_destroy(self);
        return NULL;
    }
    printf("Obtive 'recognize_command' em: [%p]\n", self->recognize);
    
    dlerror();
    *(void **) (&(self->start)) = dlsym(self->lib, "start");
    if ((error = dlerror()) != NULL) {
        printf("%s\n", dlerror());
        handler_destroy(self);
        return NULL;
    }
    printf("Obtive 'start' em: [%p]\n", self->start);
    
    dlerror();
    *(void **) (&(self->stop)) = dlsym(self->lib, "stop");
    if ((error = dlerror()) != NULL) {
        printf("%s\n", dlerror());
        handler_destroy(self);
        return NULL;
    }
    printf("Obtive 'stop' em: [%p]\n", self->stop);

    return self;
}

void handler_destroy(gpointer handler) {
    speech_handler_t* self = (speech_handler_t*) handler;

    if (self) {
        if (self->instance) {
            printf("Liberando memória da instância [%p]\n", self->instance);
            if (self->destroy) {
                printf("Invocando 'destroy' [%p]\n", self->destroy);
                (*(self->destroy))(self->instance);
            } else {
                printf("Forçando com 'free()'\n");
                free(self->instance);
            }
        }

        if (self->lib) {
            printf("Descarregando biblioteca dinâmmica [%p]\n", self->lib);
        }

        printf("Liberando memória do speech::handler [%p]\n", self);
        free(self);
    }
}
