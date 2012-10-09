#include <glib.h>
#include <stdio.h>

#define CAST_CHAR_PTR(a) ((char*)a)
#define CAST_INT_PTR( b) (( int*)b)

void my_print ( gpointer key, gpointer value, gpointer data ) {
    printf("%s: %d\n", CAST_CHAR_PTR( key ), *(CAST_INT_PTR(value)) );
}

int main( int argc, char** argv ) {
    int a = 42;
    int b = 13;

    GHashTable* table = g_hash_table_new_full( NULL, NULL, NULL, NULL );

    printf("H: %p\na: %p\n", table, &a);

    g_hash_table_insert(table, "bla bla", &a );
    g_hash_table_insert(table, "ble", &b );

    g_hash_table_foreach( table, my_print, NULL );

    gpointer key, value;

    key = "bla bla";
    value = g_hash_table_lookup( table, key );
    printf("%s: %d\n", CAST_CHAR_PTR( key ), *(CAST_INT_PTR(value)) );

    g_hash_table_destroy( table );

    return 0;
}
