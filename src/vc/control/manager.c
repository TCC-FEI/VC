#include <vc/control/manager.h>
#include <vc/control/handler.h>

#include <string.h>
#include <dlfcn.h>

gchar* _plugin_path(gchar*, gchar*);
void _list_commands( gpointer, gpointer, gpointer);
void _list_plugins( gpointer, gpointer, gpointer);

gpointer control_manager_create(gpointer data) {
    control_manager_args_t* args = NULL;
    control_manager_t* self = NULL;

    vc_trace("\nControlador inicializando...\n");

    args = (control_manager_args_t*) data;
    if (!args) {
        vc_trace("Argumentos inválidos. Abortando...\n");
        return NULL;
    }

    self = g_try_new0(control_manager_t, 1);
    if (!self) {
        vc_trace("Falha ao alocar memória para o controlador\n");
        return NULL;
    }

    vc_trace("%d bytes alocados: [%p]\n", sizeof(control_manager_t), self);

    self->plugins = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
        control_handler_destroy);
    if (!self->plugins) {
        vc_trace("Falha ao inicializar tabela de plugins\n");
        control_manager_destroy(self);
        return NULL;
    }

    vc_trace_hash("Tabela de plugins", self->plugins);

    self->plugin_dir = args->plugin_dir;
    vc_trace("Diretótio de plugins: [%s]\n", self->plugin_dir);

    self->vcwm = args->vcwm;
    vc_trace("Display obtido:    [%p]\n", self->vcwm.display);
    vc_trace("WnckScreen obtida: [%p]\n", self->vcwm.screen);

    self->execute = control_manager_execute;

    return self;
}

void control_manager_destroy(gpointer data) {
    control_manager_t* self = (control_manager_t*) data;

    vc_trace("\nControlador finalizando...\n");
    if (self) {
        if (self->plugins) {
            vc_trace("Encerrando tabela de plugins [%p]\n", self->plugins);
            g_hash_table_destroy(self->plugins);
        }

        vc_trace("Liberando memória do controlador [%p]\n", self);
        g_free(self);
    }

    vc_trace("Controlador finalizado\n");
}

void control_manager_list(gpointer data) {
    control_manager_t* self = (control_manager_t*) data;

    printf("Plugins de controle: %d\n", g_hash_table_size(self->plugins));
    g_hash_table_foreach(self->plugins, _list_plugins, NULL);
}

gboolean control_manager_load(gpointer data, gchar* plugin_name) {
    gchar* plugin_path = NULL;
    control_handler_t* handler = NULL;
    control_manager_t* self = (control_manager_t*) data;

    if (!self) {
        vc_trace("control_manager_load: self inválido\n");
        return FALSE;
    }

    if (!plugin_name) {
        vc_trace("control_manager_load: plugin_name inválido\n");
        return FALSE;
    }

    if (g_hash_table_lookup(self->plugins, plugin_name)) {
        vc_trace("control_manager_load: plugin já carregado\n");
        return TRUE;
    }

    plugin_path = _plugin_path(self->plugin_dir, plugin_name);
    if (!strlen(plugin_path)) {
        vc_trace("Arquivo de plugin inválido\n");
        return FALSE;
    }
    vc_trace("control_manager_load: plugin_name: [%s]\n", plugin_name);
    vc_trace("control_manager_load: plugin_path: [%s]\n", plugin_path);

    handler = control_handler_create(plugin_path);
    if (!handler) {
        vc_trace("Falha ao criar handler do plugin de controle\n");
        return FALSE;
    }
    vc_trace("control_manager_load: handler: [%p]\n", handler);

    vc_trace("Liberando memória de 'plugin_path' [%p]\n", plugin_path);
    g_free(plugin_path);

    vc_trace("Inicializando plugin '%s'\n", plugin_name);
    handler->instance = (*(handler->create))(&self->vcwm);
    if (!handler->instance) {
        vc_trace("Falha ao criar instância do plugin %s\n", plugin_name);
        control_handler_destroy(handler);
        return FALSE;
    }
    vc_trace("Plugin %s alocado em [%p]\n", plugin_name, handler->instance);

    (*(handler->reg_cmd))(handler->instance);

    GHashTable* commands = (*(handler->commands))(handler->instance);
    if (!commands) {
        vc_trace("Falha ao obter comandos do plugin %s\n", plugin_name);
        control_handler_destroy(handler);
        return FALSE;
    }
    vc_trace_hash("Comandos do plugin", commands);

    g_hash_table_insert(self->plugins, g_strdup(plugin_name), handler);
    vc_trace_hash("Plugins do controlador", self->plugins);

    return TRUE;
}

gboolean control_manager_unload(gpointer data, gchar* plugin_name) {
    control_manager_t* self = (control_manager_t*) data;

    if (!self) {
        vc_trace("control_manager_unload: self inválido\n");
        return FALSE;
    }

    if (!plugin_name) {
        vc_trace("control_manager_unload: plugin_name inválido\n");
        return FALSE;
    }

    g_hash_table_remove(self->plugins, plugin_name);

    return TRUE;
}

gboolean control_manager_execute(gpointer data, gchar* command) {
    control_manager_t* self = NULL;
    gboolean retval = FALSE;

    gchar** args = NULL;
    gchar* plugin_name = NULL;
    gchar* plugin_exec = NULL;
    gchar* buffer = NULL;

    control_handler_t* handler = NULL;
    GHashTable* commands = NULL;

    control_command_t cmd_cb = NULL;
    raise_t raise_cb = NULL;

    self = (control_manager_t*) data;

    if (!self) {
        vc_trace("control_manager_execute: self inválido\n");
        return FALSE;
    }

    if (!command) {
        vc_trace("control_manager_execute: command inválido\n");
        return FALSE;
    }

    args = g_strsplit(command, " ", 0);

    printf(">>>1(%d)\n", g_strv_length(args));

    if (g_strv_length(args) > 1 && strcmp(args[0], "run") == 0) {
        plugin_name = args[1];
        plugin_exec = g_strconcat(plugin_name, " run", NULL);

        retval = control_manager_load(self, plugin_name)
            && control_manager_execute(self, plugin_exec);
        g_free(plugin_exec);
        return retval;
    } else if (g_strv_length(args) > 2
        && strcmp(args[0], CTRL_PLUGIN_DEFAULT) == 0
        && strcmp(args[1], "run") == 0) {
        plugin_name = args[2];
        plugin_exec = g_strconcat(args[2], " run", NULL);

        retval = control_manager_load(self, plugin_name)
            && control_manager_execute(self, plugin_exec);
        g_free(plugin_exec);
        return retval;
    } else {
        plugin_name = args[0];
    }

    printf(">>>2\n");

    handler = g_hash_table_lookup(self->plugins, plugin_name);
    if (!handler) {
        vc_trace("Plugin '%s' não encontrado\n", plugin_name);
        plugin_name = CTRL_PLUGIN_DEFAULT;

        handler = g_hash_table_lookup(self->plugins, plugin_name);
        if (!handler) {
            vc_trace("Plugin default '%s' não encontrado, abortando\n",
                plugin_name);

            vc_trace("Liberando memória alocada para 'args'\n");
            g_strfreev(args);

            return FALSE;
        }

        plugin_exec = g_strdup(command);
        vc_trace("Plugin '%s' encontrado em [%p]\n", plugin_name, handler);
        vc_trace("'%s' executará '%s'\n", plugin_name, plugin_exec);
    } else {
        if (g_strv_length(args) > 1) {
            plugin_exec = g_strjoinv(" ", &args[1]);
            vc_trace("Plugin '%s' encontrado em [%p]\n", plugin_name,
                handler);
            vc_trace("'%s' executará '%s'\n", plugin_name, plugin_exec);
        } else {
            plugin_exec = g_strdup("");
            vc_trace("Plugin '%s' encontrado em [%p]\n", plugin_name,
                handler);
            vc_trace("'%s' executará '%s'\n", plugin_name, plugin_exec);
        }
    }

    commands = (*(handler->commands))(handler->instance);
    if (commands) {
        vc_trace("Obtive tabela de comandos do plugin '%s' em [%p]\n",
            plugin_name, commands);

        cmd_cb = g_hash_table_lookup(commands, plugin_exec);
        if (cmd_cb) {
            vc_trace("Obtive comando '%s' em [%p]\n", plugin_exec, cmd_cb);

            raise_cb = handler->raise;
            if (raise_cb) {
                (*raise_cb)(handler->instance);
                usleep(40);
                (*cmd_cb)(self->vcwm.display);

                retval = TRUE;
            } else {
                vc_trace("Falha ao obter 'raise'\n");
            }
        } else {
            vc_trace("Falha ao obter callback do comando '%s'\n",
                plugin_exec);
        }
    } else {
        vc_trace("Falha ao obter a tabela de comandos do plugin '%s'\n",
            plugin_name);
    }

    vc_trace("Liberando memória alocada para 'args'\n");
    g_strfreev(args);

    vc_trace("Liberando memória alocada para 'plugin_exec'\n");
    g_free(plugin_exec);

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

gchar* _plugin_path(gchar* plugin_dir, gchar* plugin_name) {
    return g_strconcat(plugin_dir, "lib", plugin_name, "-",
        VCC_PLUGIN_API_VERSION, ".so", NULL);
}
