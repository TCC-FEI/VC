#include <control/manager.h>
#include <control/handler.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

control_manager_t* control_manager_create(vcwm_t vcwm ) {
    control_manager_t* self;

    self = (control_manager_t*) malloc(sizeof(control_manager_t));

    if (self == NULL) {
        return NULL;
    }

    self->plugins = g_hash_table_new_full(g_str_hash, g_str_equal, NULL,
        handler_destroy);

    self->plugin_dir = "/home/aluno/workspace/VC/src/control/plugins/";

    self->vcwm = vcwm;

    printf("Control Manager:  [%p]\n", self);
    printf("Plugin Table:     [%p]\n", self->plugins);
    printf("Plugin Directory: [%s]\n", self->plugin_dir);
    printf("X11 Display:      [%p]\n", self->vcwm.display);
    printf("WnckScreen:       [%p]\n", self->vcwm.screen);

    return self;
}

void control_manager_destroy(control_manager_t* self) {
    if (self) {
        if (self->plugins) {
            g_hash_table_destroy(self->plugins);
        }

        free(self);
    }
}

gboolean control_manager_load(control_manager_t* self, gchar* plugin_name) {
    gchar* path;
    char* error;
    control_handler_t* handler;

    path = g_strconcat(self->plugin_dir, "lib", plugin_name, ".so", NULL);
    if (!strlen(path)) {
        return FALSE;
    }

    handler = handler_create();
    if (!handler) {
        fprintf(stderr, "Falha ao criar handler do plugin de controle\n");
        return FALSE;
    }

    handler->lib = dlopen(path, RTLD_LAZY);
    if (!handler->lib) {
        fprintf(stderr, "%s\n", dlerror());
        return FALSE;
    }
    g_free(path);

    dlerror();
    *(void **) (&(handler->create)) = dlsym(handler->lib, "create");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        return FALSE;
    }

    dlerror();
    *(void **) (&(handler->destroy)) = dlsym(handler->lib, "destroy");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        return FALSE;
    }

    dlerror();
    *(void **) (&(handler->commands)) = dlsym(handler->lib, "commands");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        return FALSE;
    }

    dlerror();
        *(void **) (&(handler->raise)) = dlsym(handler->lib, "raise_window");
        if ((error = dlerror()) != NULL) {
            fprintf(stderr, "%s\n", error);
            return FALSE;
        }

    handler->instance = (*(handler->create))(self->vcwm);

    fprintf(stdout, "Lib:      [%p]\n", handler->lib);
    fprintf(stdout, "Instance: [%p]\n", handler->instance);
    fprintf(stdout, "Create:   [%p]\n", handler->create);
    fprintf(stdout, "Destroy:  [%p]\n", handler->destroy);
    fprintf(stdout, "Commands: [%p]\n", handler->commands);
    fprintf(stdout, "Raise:    [%p]\n", handler->raise);

    GHashTable* commands = (*(handler->commands))(handler->instance);

    fprintf(stdout, "Num Cmds: [%d]\n", g_hash_table_size(commands));

    g_hash_table_insert(self->plugins, plugin_name, handler);

    return TRUE;
}

gboolean control_manager_unload(control_manager_t* self, gchar* plugin_name) {
    if (self) {
        control_handler_t* handler;

        handler = g_hash_table_lookup(self->plugins, plugin_name);
        (*(handler->destroy))(handler->instance);
        dlclose(handler->lib);
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

    if (handler = g_hash_table_lookup(self->plugins, plugin)) {
        if (commands = (*(handler->commands))(handler->instance)) {
            if (cc_cb = g_hash_table_lookup(commands, cmd)) {
                (*(handler->raise))(handler->instance);
                sleep(1);
                (*cc_cb)(self->vcwm.display);
                retval = TRUE;
            }
        }
    }

    g_free(cmd);
    g_strfreev(args);

    return retval;
}
