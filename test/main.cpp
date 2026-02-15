#include <core/frame/app.hpp>
#include <SDL3/SDL.h>
#include "hello_dao_page.hpp"

// ========= 上下文对象 =========
struct TrayContext {
    bool autoStart = false;
    bool mute = false;

    // radio 组（Mode）
    SDL_TrayEntry* modeNormal = nullptr;
    SDL_TrayEntry* modeGaming = nullptr;
    SDL_TrayEntry* modeSilent = nullptr;
};

// ========= 回调 =========
static void OnTrayAction(void* userdata, SDL_TrayEntry* entry)
{
    auto* ctx = static_cast<TrayContext*>(userdata);
    const char* label = SDL_GetTrayEntryLabel(entry);

    if (!label) return;

    // ---- 普通按钮 ----
    if (SDL_strcmp(label, "Open") == 0) {
        SDL_Log("Open window");
        return;
    }

    if (SDL_strcmp(label, "Exit") == 0) {
        SDL_Event quit{};
        quit.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quit);
        return;
    }

    // ---- checkbox ----
    if (SDL_strcmp(label, "Auto Start") == 0) {
        ctx->autoStart = !ctx->autoStart;
        SDL_SetTrayEntryChecked(entry, ctx->autoStart);
        SDL_Log("AutoStart: %d", ctx->autoStart);
        return;
    }

    if (SDL_strcmp(label, "Mute") == 0) {
        ctx->mute = !ctx->mute;
        SDL_SetTrayEntryChecked(entry, ctx->mute);
        SDL_Log("Mute: %d", ctx->mute);
        return;
    }

    // ---- radio 组（Mode）----
    if (entry == ctx->modeNormal ||
        entry == ctx->modeGaming ||
        entry == ctx->modeSilent)
    {
        // 手动互斥
        SDL_SetTrayEntryChecked(ctx->modeNormal, entry == ctx->modeNormal);
        SDL_SetTrayEntryChecked(ctx->modeGaming, entry == ctx->modeGaming);
        SDL_SetTrayEntryChecked(ctx->modeSilent, entry == ctx->modeSilent);

        SDL_Log("Mode changed: %s", label);
        return;
    }

    SDL_Log("Clicked: %s", label);
}

int main(int argc, char *argv[])
{
    /// 创建应用
    auto &app = dao::App::getApp();

    /// 创建窗口
    app.createWindow(300, 300, true)
        .addPage(std::make_unique<HelloDaoPage>());

    /// 创建托盘
    auto tray = app.createTray("./assets/textures/atlas/icon.png", "托盘").getSDLTray();

    // ========= 创建菜单 =========
    SDL_TrayMenu* menu = SDL_CreateTrayMenu(tray);

    TrayContext ctx{};

    // ---- Open ----
    SDL_TrayEntry* open =
        SDL_InsertTrayEntryAt(menu, -1, "Open", SDL_TRAYENTRY_BUTTON);
    SDL_SetTrayEntryCallback(open, OnTrayAction, &ctx);

    // ---- 分隔线（disabled entry 实现）----
    SDL_InsertTrayEntryAt(menu, -1, "------",
        SDL_TRAYENTRY_BUTTON | SDL_TRAYENTRY_DISABLED);

    // ---- checkbox ----
    SDL_TrayEntry* autoStart =
        SDL_InsertTrayEntryAt(menu, -1, "Auto Start", SDL_TRAYENTRY_CHECKBOX);
    SDL_SetTrayEntryCallback(autoStart, OnTrayAction, &ctx);

    SDL_TrayEntry* mute =
        SDL_InsertTrayEntryAt(menu, -1, "Mute", SDL_TRAYENTRY_CHECKBOX);
    SDL_SetTrayEntryCallback(mute, OnTrayAction, &ctx);

    // ---- submenu：Mode ----
    SDL_TrayEntry* modeEntry =
        SDL_InsertTrayEntryAt(menu, -1, "Mode", SDL_TRAYENTRY_SUBMENU);

    SDL_TrayMenu* modeMenu = SDL_CreateTraySubmenu(modeEntry);

    ctx.modeNormal =SDL_InsertTrayEntryAt(modeMenu, -1, "Normal", SDL_TRAYENTRY_CHECKBOX);

    ctx.modeGaming =SDL_InsertTrayEntryAt(modeMenu, -1, "Gaming", SDL_TRAYENTRY_CHECKBOX);

    ctx.modeSilent =SDL_InsertTrayEntryAt(modeMenu, -1, "Silent", SDL_TRAYENTRY_CHECKBOX);

    SDL_SetTrayEntryCallback(ctx.modeNormal, OnTrayAction, &ctx);
    SDL_SetTrayEntryCallback(ctx.modeGaming, OnTrayAction, &ctx);
    SDL_SetTrayEntryCallback(ctx.modeSilent, OnTrayAction, &ctx);

    // 默认选中 Normal
    SDL_SetTrayEntryChecked(ctx.modeNormal, true);

    // ---- Exit ----
    SDL_TrayEntry* exitEntry =
        SDL_InsertTrayEntryAt(menu, -1, "Exit", SDL_TRAYENTRY_BUTTON);
    SDL_SetTrayEntryCallback(exitEntry, OnTrayAction, &ctx);

    /// 运行应用
    app.run();

    return 0;
}
