#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

// Function: Click Key (press and release key)
void click_key( Display* p_display , KeyCode keycode ) {

    XTestFakeKeyEvent( p_display , keycode , True  , 0 ); // key press event
    XTestFakeKeyEvent( p_display , keycode , False , 0 ); // key release event
    XFlush( p_display );

    return;
}

void combine_key( Display* p_display , KeyCode keycode1, KeyCode keycode2 ) {

    XTestFakeKeyEvent( p_display , keycode1 , True  , 0 ); // key press event
    XTestFakeKeyEvent( p_display , keycode2 , True  , 0 ); // key press event
    XTestFakeKeyEvent( p_display , keycode1 , False , 0 ); // key release event
    XTestFakeKeyEvent( p_display , keycode2 , False , 0 ); // key release event
    XFlush( p_display );

    return;
}

int main() {

    Display* p_display = XOpenDisplay( NULL );
    KeySym keysym = XK_VoidSymbol;
    KeyCode keycode1 = NoSymbol;
    KeyCode keycode2 = NoSymbol;

    // abre o help
    //keysym   = XK_F1;
    //keycode1 = XKeysymToKeycode( p_display , keysym );
    //click_key( p_display , keycode1 );

    // pequeno delay, sabe como é ...
    // o desempenho de c é demais ...
    sleep(3);

    // criando combinacao de keys
    keysym   = XK_Alt_L;
    keycode1 = XKeysymToKeycode( p_display , keysym );
    keysym   = XK_Tab;
    keycode2 = XKeysymToKeycode( p_display , keysym );

    combine_key( p_display, keycode1, keycode2 );
    sleep(3);
    keysym   = XK_Control_L;
    keycode1 = XKeysymToKeycode( p_display , keysym );
    keysym   = XK_F;
    keycode2 = XKeysymToKeycode( p_display , keysym );
    combine_key( p_display, keycode1, keycode2 );

    XCloseDisplay( p_display );
    return 0;
}
