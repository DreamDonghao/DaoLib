#import <Cocoa/Cocoa.h>

extern "C" void dao_window_set_click_through(void *nsWindowPtr, bool enable) {
    NSWindow *nsWindow = (__bridge
    NSWindow *
    )
    nsWindowPtr;
    [nsWindow setIgnoresMouseEvents:enable];
}
