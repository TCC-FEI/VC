#include <control/control_plugin.h>

#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <stdio.h>


int file_new( Display* display );
int file_close( Display* display );


control_plugin_t* create_plugin( Display* display ) {
    control_plugin_t* plugin = NULL;

    plugin = ( control_plugin_t* ) malloc( sizeof( control_plugin_t ) );

    if ( plugin == NULL ) {
        return NULL;
    }

    plugin->commands = g_hash_table_new( g_str_hash, g_str_equal );

    if ( plugin->commands == NULL ) {
        free( plugin );
        return NULL;
    }

    plugin->display = display;

    register_commands(plugin);

    return plugin;
}

void destroy_plugin( control_plugin_t* plugin ) {
    if ( plugin ) {
        if ( plugin->commands ) {
            g_hash_table_destroy( plugin->commands );
        }

        free( plugin );
    }
}


void register_commands( control_plugin_t* plugin ) {
    g_hash_table_insert( plugin->commands, "file new", file_new );
    g_hash_table_insert( plugin->commands, "file close", file_close );
}

int file_new( Display* display ) {
    KeyCode key_1 = XKeysymToKeycode( display, XK_Control_L );
    KeyCode key_2 = XKeysymToKeycode( display, XK_N );

    XTestFakeKeyEvent( display, key_1 , True  , 0 );
    XTestFakeKeyEvent( display, key_2 , True  , 0 );

    XTestFakeKeyEvent( display, key_1 , False , 0 );
    XTestFakeKeyEvent( display, key_2 , False , 0 );

    XFlush( display );
}

int file_close( Display* display ) {
    KeyCode key_1 = XKeysymToKeycode( display, XK_Control_L );
    KeyCode key_2 = XKeysymToKeycode( display, XK_W );

    XTestFakeKeyEvent( display, key_1 , True  , 0 );
    XTestFakeKeyEvent( display, key_2 , True  , 0 );

    XTestFakeKeyEvent( display, key_1 , False , 0 );
    XTestFakeKeyEvent( display, key_2 , False , 0 );

    XFlush( display );
}
