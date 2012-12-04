#include <vc/console/console.h>
#include <vc/console/commands.h>

#include <stdio.h>
#include <string.h>

console_t* _initialize(gpointer data);
void _finalize(console_t* self);

void _read(console_t* self);
void _execute(console_t* self);

gpointer console(gpointer data) {
    console_t* self = _initialize(data);
    GMainLoop* loop = self->app->main_loop;

    while (!self->terminate) {
        printf("vc> ");
        _read(self);
        _execute(self);
    }

    _finalize(self);

    g_main_loop_quit(loop);
    g_main_loop_unref(loop);

    return NULL;
}

console_t* _initialize(gpointer data) {
    vc_trace("\nConsole inicializando...\n");

    console_t* self = g_try_new0(console_t, 1);
    if (!self) {
        vc_trace("Falha alocando memória para console\n");
        return NULL;
    }

    vc_trace("%u bytes alocados: [%p]\n", sizeof(console_t), self);

    self->app = (app_t*) data;
    if (!data) {
        vc_trace("App inválida. Abortando...\n");
        _finalize(self);
        return NULL;
    }

    vc_trace("console::app: [%p]\n", self->app);

    self->commands = g_hash_table_new(g_str_hash, g_str_equal);
    if (!self->commands) {
        vc_trace("Falha ao inicializar tabela de comandos\n");
        _finalize(self);
        return NULL;
    }

    g_hash_table_insert(self->commands, "help", cc_help);
    g_hash_table_insert(self->commands, "exit", cc_exit);
    g_hash_table_insert(self->commands, "quit", cc_exit);
    g_hash_table_insert(self->commands, "ctrl", cc_ctrl);
    g_hash_table_insert(self->commands, "spch", cc_spch);

    vc_trace_hash("Tabela de comandos", self->commands);

    vc_trace("Inicicalizando buffer... ");
    self->buffer = g_strnfill(MAX_BUFF_SIZE, '\0');
    if (!self->buffer) {
        vc_trace("Falha ao inicializar buffer\n");
        _finalize(self);
        return NULL;
    }

    vc_trace("%d bytes: [%p]\n", MAX_BUFF_SIZE, self->buffer);

    self->terminate = FALSE;
    vc_trace("Flag de fim da thread: %d\n", self->terminate);

    /**
     * Plugins iniciais
     */
    control_manager_load( self->app->control, "computer" );
    speech_manager_load( self->app->speech, "sphinx" );
    gchar** args = g_strsplit("spch start", " ", 0);
    cc_spch(self, args);
    g_strfreev(args);

    return self;
}

void _finalize(console_t* self) {
    vc_trace("\nConsole finalizando...\n");

    if (self) {
        if (self->buffer) {
            vc_trace("Encerrando buffer             [%p]\n", self->buffer);
            g_free(self->buffer);
        }

        if (self->commands) {
            vc_trace("Encerrando tabela de comandos [%p]\n", self->commands);
            g_hash_table_destroy(self->commands);
        }

        vc_trace("Liberando memória do console  [%p]\n", self);
        g_free(self);
    }

    vc_trace("Console finalizado\n");
}

void _read(console_t* self) {
    if (fgets(self->buffer, MAX_BUFF_SIZE, stdin)) {
        self->buffer[strlen(self->buffer) - 1] = '\0';
        vc_trace("console::_read() leu: [%s]\n", self->buffer);
    } else {
        vc_trace("Falha lendo comando no console\n");
    }
}

void _execute(console_t* self) {
    if (strlen(self->buffer) > 0) {
        vc_trace("console::_execute() buffer: [%s]\n", self->buffer);
        gchar** args = g_strsplit(self->buffer, " ", 0);

        console_cmd_ptr cmd = g_hash_table_lookup(self->commands, args[0]);

        if (cmd) {
            vc_trace("Comando '%s' alocado em [%p]\n", args[0], cmd);
            (*cmd)(self, args);
        } else {
            printf("Comando '%s' não encontrado\n", args[0]);
        }

        g_strfreev(args);
    } else {
        vc_trace("Buffer vazio. Ignorando...\n");
    }
}
