#include <utils.h>

void _vc_print_hash(gpointer key, gpointer value, gpointer data) {
    printf("\tKey: '%s', Value: [%p]\n", (char*) key, value);
}

void _vc_trace_hash(gchar* name, GHashTable* hash) {
    printf("%s: [%p]:\n", name, hash);
    g_hash_table_foreach(hash, _vc_print_hash, NULL);
}

void _vc_trace_strv(gchar* name, gchar** strv) {
    int i;

    printf("%s: [%p]:\n", name, strv);
    for (i = 0; i < g_strv_length(strv); i++) {
        printf("\t'%s'\n", strv[i]);
    }
}
