#include <AppTray.hpp>

void AppTray::createTrayMenu() {
    button("设置", settings);
    button("退出", quit);
}
