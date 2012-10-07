#include <control/control_plugin.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <stdio.h>

void my_print ( gpointer key, gpointer value, gpointer data ) {
    printf("%s: %p\n", (char*)key, value );
}

int main( int argc, char** argv ) {
    control_plugin_t* plugin = create_plugin( XOpenDisplay( NULL ) );

    cmd_func_t cmd_ptr;

    cmd_ptr = g_hash_table_lookup( plugin->commands, "file new" );

    sleep(5);
    (*cmd_ptr)( plugin->display );

    cmd_ptr = g_hash_table_lookup( plugin->commands, "file close" );
    sleep(5);
    (*cmd_ptr)( plugin->display );


    g_hash_table_foreach( plugin->commands, my_print, NULL );

    destroy_plugin( plugin );

    return 0;
}
