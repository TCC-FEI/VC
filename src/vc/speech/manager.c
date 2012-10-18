#include <speech/manager.h>
#include <speech/handler.h>

#include <string.h>
#include <glib.h>
#include <dlfcn.h>

speech_manager_t* speech_manager_create(int* argc, char*** argv) {
    speech_manager_t* self;
    
    self = (speech_manager_t*) malloc(sizeof(speech_manager_t));
    
    if(!self){
        printf("Falha ao alocar memoria para o controlador\n");
        return NULL;
    }
    
    printf("%d bytes alocados: [%p]\n", sizeof(speech_manager_t), self);
    
    self->argc       = argc;
    self->argv       = argv;
    self->plugin_dir = "/home/aluno/workspace/Sphinx/speech/plugins/";
    
    printf("Diretótio de plugins: [%s]\n", self->plugin_dir);
    
    return self;
}

void speech_manager_destroy(speech_manager_t* self) {
    printf("\nControlador finalizando...\n");
    
    if(self){
        printf("Liberando memoria do contralor [%p]\n", self);
        free(self);
    }
    
    printf("Controlador finalizado\n");
}

gboolean speech_manager_load(speech_manager_t* self, gchar* plugin_name) {
    gchar* path;
    speech_handler_t* handler;
    
    path = g_strconcat(self->plugin_dir, "lib", plugin_name, ".so", NULL);
    
    printf("speech::manager::load(): plugin: [%s]\n", plugin_name);
    printf("speech::manager::load(): path:   [%s]\n", path);
    if (!strlen(path)) {
        printf("Arquivo de plugin inválido\n");
        return FALSE;
    }
    
    handler = handler_create(path);
    printf("speech::manager::load(): handler: [%p]\n", handler);
    if (!handler) {
        printf("Falha ao criar handler do plugin de fala\n");
        return FALSE;
    }
    
    printf("Liberando memória da variável 'path' [%p]\n", path);
    g_free(path);
    
    handler->instance = (*(handler->create))(self->argc, self->argv);
    if (!handler->instance) {
        printf("Falha ao criar instância do plugin %s\n", plugin_name);
        handler_destroy(handler);
        return FALSE;
    }
    printf("Criada instância do plugin %s em [%p]\n", plugin_name,
        handler->instance);
        
    self->handler = handler;
    
    return TRUE;
}

gboolean speech_manager_unload(speech_manager_t* self) {
    if (self) {
        handler_destroy(self);
        return TRUE;
    } else {
        return FALSE;
    }
}
