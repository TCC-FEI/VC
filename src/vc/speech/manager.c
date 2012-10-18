#include <vc/speech/manager.h>
#include <vc/speech/handler.h>

#include <string.h>
#include <glib.h>
#include <dlfcn.h>

speech_manager_t* speech_manager_create() {
    speech_manager_t* self;

    self = (speech_manager_t*) malloc(sizeof(speech_manager_t));

    if (!self) {
        vc_trace("Falha ao alocar memoria para o controlador\n");
        return NULL;
    }

    vc_trace("%d bytes alocados: [%p]\n", sizeof(speech_manager_t), self);

    self->plugin_dir = "/tmp/lib/plugins/";

    vc_trace("Diretótio de plugins: [%s]\n", self->plugin_dir);

    return self;
}

void speech_manager_destroy(speech_manager_t* self) {
    vc_trace("\nControlador finalizando...\n");

    if (self) {
        vc_trace("Liberando memoria do contralor [%p]\n", self);
        free(self);
    }

    vc_trace("Controlador finalizado\n");
}

gboolean speech_manager_load(speech_manager_t* self, gchar* plugin_name) {
    gchar* path;
    speech_handler_t* handler;

    path = g_strconcat(self->plugin_dir, "lib", plugin_name, "-", VCC_PLUGIN_API_VERSION, ".so", NULL);

    vc_trace("vc::speech::manager::load(): plugin: [%s]\n", plugin_name);
    vc_trace("vc::speech::manager::load(): path:   [%s]\n", path);
    if (!strlen(path)) {
        vc_trace("Arquivo de plugin inválido\n");
        return FALSE;
    }

    handler = speech_handler_create(path);
    vc_trace("vc::speech::manager::load(): handler: [%p]\n", handler);
    if (!handler) {
        vc_trace("Falha ao criar handler do plugin de fala\n");
        return FALSE;
    }

    vc_trace("Liberando memória da variável 'path' [%p]\n", path);
    g_free(path);

    handler->instance = (*(handler->create))();
    if (!handler->instance) {
        vc_trace("Falha ao criar instância do plugin %s\n", plugin_name);
        speech_handler_destroy(handler);
        return FALSE;
    }
    vc_trace("Criada instância do plugin %s em [%p]\n", plugin_name,
        handler->instance);

    self->handler = handler;

    return TRUE;
}

gboolean speech_manager_unload(speech_manager_t* self) {
    if (self) {
        speech_handler_destroy(self);
        return TRUE;
    } else {
        return FALSE;
    }
}
