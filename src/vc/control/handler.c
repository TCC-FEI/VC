#include <vc/control/handler.h>

#include <dlfcn.h>

control_handler_t* control_handler_create(gchar* filename) {
    control_handler_t* self;
    char* error;

    self = g_try_new0(control_handler_t, 1);
    if (!self) {
        vc_trace("Falha ao alocar memória para control_handler\n");
        return NULL;
    }

    self->lib = dlopen(filename, RTLD_LAZY);
    vc_trace("Carregando biblioteca dinâmica em [%p]\n", self->lib);
    if (!self->lib) {
        vc_trace("%s\n", dlerror());
        control_handler_destroy(self);
        return NULL;
    }

    dlerror();
    *(void **) (&(self->create)) = dlsym(self->lib, "create");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        control_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'create' em:       [%p]\n", self->create);

    dlerror();
    *(void **) (&(self->destroy)) = dlsym(self->lib, "destroy");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        control_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'destroy' em:      [%p]\n", self->destroy);

    dlerror();
    *(void **) (&(self->reg_cmd)) = dlsym(self->lib, "register_commands");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        control_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'register_commands' em: [%p]\n", self->reg_cmd);

    dlerror();
    *(void **) (&(self->commands)) = dlsym(self->lib, "commands");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        control_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'commands' em:     [%p]\n", self->commands);

    dlerror();
    *(void **) (&(self->raise)) = dlsym(self->lib, "raise_window");
    if ((error = dlerror()) != NULL) {
        vc_trace("%s\n", dlerror());
        control_handler_destroy(self);
        return NULL;
    }
    vc_trace("Obtive 'raise_window' em: [%p]\n", self->raise);

    return self;
}

void control_handler_destroy(gpointer handler) {
    control_handler_t* self = (control_handler_t*) handler;

    if (self) {
        if (self->instance) {
            vc_trace("Liberando memória da instância [%p]\n", self->instance);
            if (self->destroy) {
                vc_trace("Invocando 'destroy' [%p]\n", self->destroy);
                (*(self->destroy))(self->instance);
            } else {
                vc_trace("Forçando desalocação 'g_free()'\n");
                g_free(self->instance);
            }
        }

        if (self->lib) {
            vc_trace("Descarregando biblioteca dinâmmica [%p]\n", self->lib);
            dlclose(self->lib);
        }

        vc_trace("Liberando memória do control::handler [%p]\n", self);
        g_free(self);
    }
}
