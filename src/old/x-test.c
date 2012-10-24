#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    Display* display;
    Screen* screen;
    Window root_window;

    int i;

    display = XOpenDisplay(NULL);

    if (!display) {
        fprintf(stderr, "Cannot open display");
        return -1;
    }

    screen = XDefaultScreenOfDisplay(display);
    root_window = XRootWindow(display, XScreenNumberOfScreen(screen));

    Window root_return;
    Window parent_return;
    Window* children_return;
    int nchildren_return;

    XQueryTree(display, root_window, &root_return, &parent_return,
        &children_return, &nchildren_return);

    char* name;
    for (i = 0; i < nchildren_return; i++) {
        XFetchName(display, children_return[i], &name);
        if (name)
            printf("[%d](%s){%lu}\n", i, name, (unsigned long)children_return[i]);
    }

    //XCirculateSubwindowsUp( display, root_window );

    XCloseDisplay(display);
    return 0;
}
