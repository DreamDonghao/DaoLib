@page core 核心框架参考 {#core}

[TOC]

# 核心框架参考 — App / Window / Context / FrameLimiter / Tray

---

## App — 应用程序入口 {#app}

`dao::App` 管理整个应用的生命周期，每个进程应有且仅有一个 App 实例。

@ref quickstart "→ 返回教程：快速开始"

### 头文件

```cpp
#include <core/frame/App.hpp>
```

### 构造函数

```cpp
explicit App(i32 fps = 60, bool clickThrough = false);
```

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| `fps` | `i32` | `60` | 目标帧率 |
| `clickThrough` | `bool` | `false` | 失焦时鼠标事件是否穿透 |

### 方法

#### createWindow

```cpp
Window &createWindow(i32 width, i32 height, std::string_view tag,
                     Window::WorkState workState,
                     bool isSubject = false, bool resizable = false,
                     bool transparent = false, bool onTop = false,
                     bool borderless = false);
```

创建并注册一个窗口。参数详见 @ref window 。

#### createTray

```cpp
template<typename Type>
Tray &createTray();
```

创建系统托盘（模板参数为 Tray 子类）。

#### getWindow

```cpp
Window &getWindow(i32 windowId);
```

通过窗口 ID 获取窗口引用。

#### showWindow / hideWindow

```cpp
void showWindow(std::string_view windowTag);
void hideWindow(std::string_view windowTag);
```

通过标签显示/隐藏窗口。

#### run

```cpp
void run();
```

启动主事件循环（阻塞直到 `exit()` 被调用）。

#### exit

```cpp
void exit();
```

退出应用主循环。

#### getContext

```cpp
Context &getContext();
```

获取全局上下文对象。典型用法：

```cpp
app.getContext().emplace<App>(app);  // 将 App 注入 Context
```

---

## Window — 窗口管理 {#window}

`dao::Window` 封装 SDL_Window，管理页面和渲染。

@ref quickstart "→ 返回教程：快速开始"

### 头文件

```cpp
#include <core/frame/Window.hpp>
```

### WorkState 枚举

| 值 | 说明 |
|----|------|
| `Foreground` | 前台运行 — 窗口正常显示和更新 |
| `Background` | 后台运行 — 窗口隐藏但仍执行 update |
| `Closed` | 窗口关闭 — 窗口不运行 |

### addPage

```cpp
// 模板版本（推荐）：参数转发给页面构造函数
template<typename PageType, typename... Args>
Window &addPage(Args &&... args);

// unique_ptr 版本
Window &addPage(std::unique_ptr<ifc::IPage> &&page);
```

返回 `Window&` 支持链式调用：

```cpp
app.createWindow(800, 600, "main", Window::WorkState::Foreground)
    .addPage<HomePage>()
    .addPage<SettingsPage>();
```

### switchPage

```cpp
void switchPage(std::string title);
```

切换到指定标题的页面。

### 窗口控制

```cpp
void show() const;                        // 显示窗口
void hide() const;                        // 隐藏窗口
void setPosition(i32 x, i32 y) const;     // 设置位置
void movePosition(i32 x, i32 y) const;    // 移动（增量）
void setSize(i32 width, i32 height) const;// 设置大小
void setTitle(const std::string &title) const;  // 设置标题
void setClickThrough(bool enable) const;  // 设置点击穿透
```

### 状态查询

```cpp
i32 getId() const;                        // 窗口 ID
WorkState workState() const;              // 当前工作状态
const std::string &getNowPageTitle() const; // 当前页面标题
const SDL_Window *getSDLWindow() const;   // SDL 窗口指针
```

### 工作状态转换

```cpp
void convertWorkState(WorkState workState);
```

可在运行时切换窗口的工作状态。

---

## Context — 上下文管理器 {#context}

`Context` 提供类型安全的泛型容器，用于依赖注入和跨组件数据共享。

@ref context "→ 返回教程：Context"

### 头文件

```cpp
#include <core/frame/Context.hpp>
```

### emplace — 存储对象

```cpp
template<typename T, typename... Args>
T &emplace(Args &&... args);
```

两种模式：

```cpp
// 引用模式：传入单个 T 类型对象，不持有所有权
App &app = ...;
context.emplace<App>(app);       // 仅引用，不管理生命周期

// 创建模式：传入构造参数，容器持有所有权
context.emplace<PlayerState>("Alice", 100);  // 在容器内构造
```

### get — 获取对象

```cpp
template<typename T>
T *get();
```

返回指针，不存在时返回 `nullptr` 并输出错误日志。

### has — 检查是否存在

```cpp
template<typename T>
bool has() const;
```

### 典型模式

```cpp
// main() 中注入 App
app.getContext().emplace<App>(app);

// 页面中获取
auto &app = *getContext().get<App>();
```

---

## FrameLimiter — 帧率限制器 {#framelimiter}

`FrameLimiter` 在 App 内部自动使用，采用"睡眠+自旋"混合等待策略。

@ref framelimiter "→ 返回教程：FrameLimiter"

### 头文件

```cpp
#include <core/frame/FrameLimiter.hpp>
```

### 构造函数

```cpp
explicit FrameLimiter(i32 target_fps);
```

### 方法

```cpp
void wait();              // 等待直到下一帧
void setFPS(i32 target_fps); // 动态修改目标帧率
```

**策略**：先 `sleep_for` 等待大部分剩余时间，最后 2ms 内自旋等待以达到纳秒级精度。

---

## Tray — 系统托盘 {#tray}

`dao::Tray` 是抽象基类，管理系统托盘图标和菜单。

@ref tray "→ 返回教程：系统托盘"

### 头文件

```cpp
#include <core/frame/Tray.hpp>
```

### 构造函数

```cpp
Tray(std::string_view iconPath, std::string_view tooltip);
```

### 纯虚方法

```cpp
virtual void createTrayMenu() = 0;
```

子类必须实现此方法，在其中调用 `button()` 添加菜单项。

### button — 添加菜单项

```cpp
// 引用版本（需保证回调生命周期长于托盘）
void button(std::string_view label, TrayFunc &trayFunc) const;

// 右值版本（回调被移动存储）
void button(std::string_view label, TrayFunc &&trayFunc);
```

### 上下文访问

```cpp
Context &getContext() const;
void setContext(Context &context);
```

### 完整示例

```cpp
class AppTray : public dao::Tray {
public:
    AppTray() : Tray("icon.png", "DaoApp") {}

    void createTrayMenu() override {
        button("显示窗口", [this](SDL_TrayEntry *) {
            getContext().get<App>()->showWindow("main");
        });
        button("退出", [this](SDL_TrayEntry *) {
            getContext().get<App>()->exit();
        });
    }
};

// 在 main() 中
app.createTray<AppTray>();
```