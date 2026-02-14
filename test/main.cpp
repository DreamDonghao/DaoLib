#include <core/frame/app.hpp>
#include <SDL3_image/SDL_image.h>

#include "hello_dao_page.hpp"

static void OnTrayClicked(void *userdata, SDL_TrayEntry *entry) {
    const char *label = SDL_GetTrayEntryLabel(entry);
    SDL_Log("Clicked: %s", label);

    if (SDL_strcmp(label, "Exit") == 0) {
        SDL_Event quit{};
        quit.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quit);
    }
}

int main(int argc, char *argv[]) {
    /// 创建一个应用
    auto &app = dao::App::getApp();

    /// 创建一个窗口并添加一个页面
    app.createWindow(300, 300, true)
            .addPage(std::make_unique<HelloDaoPage>());


    SDL_Surface* icon = IMG_Load("tray.png");
    SDL_Tray* tray = SDL_CreateTray(icon, "SDL3 Tray Demo");

    if (!tray) {
        SDL_Log("Create tray failed: %s", SDL_GetError());
        SDL_DestroySurface(icon);
        SDL_Quit();
        return 1;
    }

    // ===== 创建托盘菜单 =====
    SDL_TrayMenu *menu = SDL_CreateTrayMenu(tray);

    // 添加“按钮”（菜单项）
    SDL_TrayEntry *entry1 = SDL_InsertTrayEntryAt(menu, -1, "Open", SDL_TRAYENTRY_BUTTON);
    SDL_TrayEntry *entry2 = SDL_InsertTrayEntryAt(menu, -1, "Settings", SDL_TRAYENTRY_BUTTON);
    SDL_TrayEntry *entry3 = SDL_InsertTrayEntryAt(menu, -1, "Exit", SDL_TRAYENTRY_BUTTON);

    // 绑定点击回调
    SDL_SetTrayEntryCallback(entry1, OnTrayClicked, nullptr);
    SDL_SetTrayEntryCallback(entry2, OnTrayClicked, nullptr);
    SDL_SetTrayEntryCallback(entry3, OnTrayClicked, nullptr);

    app.run();
    SDL_DestroySurface(icon);
    SDL_DestroyTray(tray);
    return 0;
}
