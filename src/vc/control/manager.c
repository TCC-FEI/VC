#include <vc/control/manager.h>
#include <vc/control/handler.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

control_manager_t* control_manager_create(vcwm_t vcwm) {
    vc_trace("\nControlador inicializando...\n");

    control_manager_t* self;

    self = (control_manager_t*) malloc(sizeof(control_manager_t));
    if (!self) {
        vc_trace("Falha ao alocar memória para o controlador\n");
        return NULL;
    }

    vc_trace("%d bytes alocados: [%p]\n", sizeof(control_manager_t), self);

    self->plugins = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
        handler_destroy);
    if (!self->plugins) {
        vc_trace("Falha ao inicializar tabela de plugins\n");
        control_manager_destroy(self);
        return NULL;
    }

    vc_trace_hash("Tabela de plugins", self->plugins);

    self->plugin_dir = "/tmp/lib/plugins/";
    vc_trace("Diretótio de plugins: [%s]\n", self->plugin_dir);

    self->vcwm = vcwm;
    vc_trace("Display obtido:    [%p]\n", self->vcwm.display);
    vc_trace("WnckScreen obtida: [%p]\n", self->vcwm.screen);

    return self;
}

void control_manager_destroy(control_manager_t* self) {
    vc_trace("\nControlador finalizando...\n");
    if (self) {
        if (self->plugins) {
            vc_trace("Encerrando tabela de plugins [%p]\n", self->plugins);
            g_hash_table_destroy(self->plugins);
        }

        vc_trace("Liberando memória do controlador [%p]\n", self);
        free(self);
    }

    vc_trace("Controlador finalizado\n");
}

gboolean control_manager_load(control_manager_t* self, gchar* plugin_name) {
    gchar* path;
    control_handler_t* handler;

    path = g_strconcat(self->plugin_dir, "lib", plugin_name, "-", VCC_PLUGIN_API_VERSION, ".so", NULL);
    vc_trace("control::manager::load(): plugin: [%s]\n", plugin_name);
    vc_trace("control::manager::load(): path:   [%s]\n", path);
    if (!strlen(path)) {
        vc_trace("Arquivo de plugin inválido\n");
        return FALSE;
    }

    handler = handler_create(path);
    vc_trace("control::manager::load(): handler: [%p]\n", handler);
    if (!handler) {
        vc_trace("Falha ao criar handler do plugin de controle\n");
        return FALSE;
    }

    vc_trace("Liberando memória da variável 'path' [%p]\n", path);
    g_free(path);

    handler->instance = (*(handler->create))(self->vcwm);
    if (!handler->instance) {
        vc_trace("Falha ao criar instância do plugin %s\n", plugin_name);
        handler_destroy(handler);
        return FALSE;
    }
    vc_trace("Criada instância do plugin %s em [%p]\n", plugin_name,
        handler->instance);

    GHashTable* commands = (*(handler->commands))(handler->instance);
    if (!commands) {
        vc_trace("Falha ao obter comandos do plugin %s\n", plugin_name);
        (*(handler->destroy))(handler->instance);
        dlclose(handler->lib);
        handler_destroy(handler);
        return FALSE;
    }
    vc_trace_hash("Comandos do plugin", commands);

    g_hash_table_insert(self->plugins, g_strdup(plugin_name), handler);
    vc_trace_hash("Plugins do controlador", self->plugins);

    return TRUE;
}

gboolean control_manager_unload(control_manager_t* self, gchar* plugin_name) {
    if (self) {
        g_hash_table_remove(self->plugins, plugin_name);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean control_manager_execute(control_manager_t* self, gchar* command) {
    gboolean retval = FALSE;
    control_handler_t* handler = NULL;
    GHashTable* commands = NULL;
    control_command_t cc_cb = NULL;

    gchar** args = g_strsplit(command, " ", 0);
    gchar* plugin = args[0];
    gchar* cmd = g_strjoinv(" ", &args[1]);

    printf("command:[%s]\n", command);
    printf("plugin: [%s]\n", plugin);
    printf("cmd:    [%s]\n", cmd);

    //g_hash_table_foreach(self->plugins, my_print, NULL);

    if (handler = g_hash_table_lookup(self->plugins, plugin)) {
        if (commands = (*(handler->commands))(handler->instance)) {
            if (cc_cb = g_hash_table_lookup(commands, cmd)) {
                (*(handler->raise))(handler->instance);
                sleep(1);
                (*cc_cb)(self->vcwm.display);
                retval = TRUE;
            } else {
                printf("callback invalida\n");
            }
        } else {
            printf("commands invalido\n");
        }
    } else {
        printf("handler invalido\n");
    }

    g_free(cmd);
    g_strfreev(args);

    return retval;
}

void _list_commands(gpointer key, gpointer value, gpointer data) {
    printf("\t\t'%s' em: [%p]\n", (char*) key, value);
}
void _list_plugins(gpointer key, gpointer value, gpointer data) {
    control_handler_t* handler = (control_handler_t*) value;
    GHashTable* commands = (*(handler->commands))(handler->instance);

    printf("\t'%s' em [%p]: %d\n", (char*) key, value,
        g_hash_table_size(commands));
    g_hash_table_foreach(commands, _list_commands, NULL);
}
void control_manager_list(control_manager_t* self) {
    printf("Plugins de controle: %d\n", g_hash_table_size(self->plugins));
    g_hash_table_foreach(self->plugins, _list_plugins, NULL);
}
