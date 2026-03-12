@page tutorial DaoLib 从零基础到精通教程

# DaoLib 从零基础到精通教程

## 简介

DaoLib 是一个基于 SDL3 的现代 C++ 用户界面库，旨在提供简单、高效、可扩展的 UI 开发体验。它采用 C++23 标准，支持跨平台开发，提供了完整的应用管理、窗口系统、页面组件、渲染批处理和状态管理等功能。

### 主要特性

- **跨平台支持**：基于 SDL3，支持 Windows、macOS、Linux
- **现代 C++**：使用 C++23 标准，提供类型安全的 API
- **组件化设计**：内置按钮、输入框、图片、文本等常用 UI 组件
- **页面管理系统**：灵活的页面切换和管理机制
- **高效渲染**：基于顶点批处理的渲染系统，支持纹理图集
- **状态管理**：统一的上下文管理器，支持状态和服务的依赖注入
- **可扩展性**：易于自定义组件和页面
- **托盘支持**：系统托盘图标和菜单

---

## 安装与构建

### 系统要求

- C++23 兼容的编译器（如 GCC 13+, Clang 16+, MSVC 2022 17.8+）
- CMake 3.31 或更高版本
- SDL3、SDL3_image、SDL3_ttf 库
- utf8cpp 库

### 使用 vcpkg 管理依赖

DaoLib 使用 vcpkg 管理依赖。确保已安装 [vcpkg](https://vcpkg.io/) 并配置好集成。

```bash
# 安装依赖
vcpkg install sdl3 sdl3-image[png] sdl3-ttf utf8cpp
```

### 集成到 CMake 项目

将 DaoLib 作为子模块或直接复制到你的项目中，然后在 `CMakeLists.txt` 中添加：

```cmake
# 添加 DaoLib 子目录
add_subdirectory(path/to/dao)

# 链接 DaoLib 库
target_link_libraries(your_target PRIVATE dao::dao)
```

### 独立构建 DaoLib

```bash
# 克隆仓库
git clone https://github.com/your-username/DaoLib.git
cd DaoLib

# 配置和构建
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path/to/vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

---

## 快速开始

### 最小应用示例

创建一个简单的窗口应用：

```cpp
#include <daoLib.hpp>

int main() {
    // 创建应用实例，60 FPS，启用点击穿透
    dao::App app{60, true};

    // 创建窗口（宽1300，高1300，标签"main"，显示，非主体，可调整大小，透明）
    auto& window = app.createWindow(1300, 1300, "main", false, false, true, true);

    // 运行应用
    app.run();

    return 0;
}
```

### 第一个完整应用

让我们创建一个包含页面和基本组件的完整应用：

```cpp
#include <daoLib.hpp>

// 自定义页面类
class MyPage : public dao::GeneralPage {
    dao::SimpleButton button{100, 100, 200, 50, [] {
        std::cout << "按钮被点击!" << std::endl;
    }};

    dao::Text text{100, 200, 24, dao::hexToRGBA("#000000"), U"你好 DaoLib!"};

public:
    MyPage() : GeneralPage("myPage") {}

    // 注册页面需要的纹理
    std::vector<dao::i32> getRegisterTexture() const override {
        return {}; // 本例不需要纹理
    }

    void init() override {
        // 页面初始化代码
    }

    void close() override {
        // 页面关闭代码
    }

    void update() override {
        // 每帧更新，添加组件到批处理
        addToBatch(button, text);
    }

    void handleInputEvent(const SDL_Event& event) override {
        // 处理输入事件
        button.handleEvent(event);
    }
};

int main() {
    dao::App app{60};

    // 创建窗口并添加页面
    app.createWindow(800, 600, "main")
        .addPage(std::make_unique<MyPage>());

    app.run();
    return 0;
}
```

---

## 核心概念

### 1. 应用 (App)

`dao::App` 是 DaoLib 应用的入口点，负责管理整个应用程序的生命周期。

**主要功能：**
- 创建和管理窗口
- 管理应用级上下文
- 控制主事件循环
- 帧率限制

**常用方法：**
```cpp
// 创建应用
dao::App app{fps, clickThrough};

// 创建窗口
auto& window = app.createWindow(width, height, tag, hidden, isSubject,
                                resizable, transparent, onTop, borderless);

// 获取上下文
auto& context = app.getContext();

// 运行应用
app.run();

// 关闭应用
app.close();
```

### 2. 窗口 (Window)

`dao::Window` 表示一个应用程序窗口，可以包含多个页面。

**窗口创建选项：**
- `width`, `height`: 窗口尺寸
- `tag`: 窗口标识（用于查找）
- `hidden`: 是否隐藏
- `isSubject`: 是否为主体窗口
- `resizable`: 是否可调整大小
- `transparent`: 是否支持透明
- `onTop`: 是否置顶
- `borderless`: 是否无边框

**窗口管理：**
```cpp
// 添加页面
window.addPage(std::make_unique<MyPage>());

// 切换页面
window.switchPage("pageTitle");

// 显示/隐藏窗口
window.show();
window.hide();

// 设置窗口属性
window.setPosition(x, y);
window.setSize(width, height);
window.setTitle("新标题");
```

### 3. 页面 (Page)

页面是 UI 内容的容器，继承自 `dao::Page` 或 `dao::GeneralPage`。

**页面生命周期方法：**
- `init()`: 页面打开时调用
- `close()`: 页面关闭时调用
- `update()`: 每帧调用，用于更新 UI
- `handleInputEvent()`: 处理输入事件
- `getRegisterTexture()`: 返回页面需要的纹理 ID 列表

**使用 GeneralPage：**
```cpp
class MyPage : public dao::GeneralPage {
public:
    MyPage() : GeneralPage("pageTitle") {}

    // 必须实现的方法
    void init() override { /* ... */ }
    void close() override { /* ... */ }
    void update() override { /* ... */ }
    void handleInputEvent(const SDL_Event& event) override { /* ... */ }
    std::vector<dao::i32> getRegisterTexture() const override { /* ... */ }

    // 辅助方法
    void updateLogic() {
        // 更新逻辑
    }

    void renderUI() {
        // 渲染 UI 组件
        addToBatch(component1, component2, component3);
    }
};
```

### 4. 组件 (Components)

DaoLib 提供了多种内置 UI 组件。

#### 按钮 (SimpleButton)

```cpp
// 创建按钮
dao::SimpleButton button{
    x, y, width, height,          // 位置和尺寸
    [] {                           // 点击回调
        std::cout << "按钮点击" << std::endl;
    },
    dao::ButtonStatus::Normal     // 初始状态
};

// 处理事件
button.handleEvent(event);

// 检查状态
if (button.isEnable()) {
    // 按钮可用
}

// 设置状态
button.setStatus(dao::ButtonStatus::Disabled);
```

#### 输入框 (InputBox)

```cpp
// 创建输入框
dao::InputBox inputBox{x, y, width, height, dao::InputStatus::Normal};

// 处理事件
inputBox.handleEvent(event);

// 获取输入内容
std::u32string text = inputBox.getText();
int cursorPos = inputBox.getCursorPos();
```

#### 文本 (Text)

```cpp
// 创建文本
dao::Text text{
    x, y,                         // 位置
    fontSize,                     // 字体大小
    dao::hexToRGBA("#FF0000"),    // 颜色
    U"文本内容"                   // UTF-32 字符串
};

// 修改文本属性
text.setContent(U"新内容");
text.setColor(dao::hexToRGBA("#00FF00"));
text.setFontSize(48.0f);
```

#### 图片 (Image)

```cpp
// 创建图片（需要预加载纹理）
dao::Image image{x, y, width, height, textureId};

// 纹理 ID 需要在页面中注册
std::vector<dao::i32> getRegisterTexture() const override {
    return {textureId};
}
```

#### 矩形 (Rectangle)

```cpp
// 创建矩形
dao::Rectangle rect{x, y, width, height, dao::hexToRGBA("#3498db")};
```

#### 纹理 (Texture)

```cpp
// 创建纹理实例
dao::Texture texture{textureId, x, y, width, height};
```

### 5. 渲染批处理 (VertexBatchBuilder)

`VertexBatchBuilder` 负责高效的批处理渲染。

**基本用法：**
```cpp
// 在页面中
void update() override {
    // 清空上一帧的批处理
    // addToBatch 内部会自动调用 clearDrawBatches()

    // 添加组件到批处理
    addToBatch(button, text, image, rect);
}

// 自定义 Drawable 对象
class CustomDrawable : public dao::Drawable {
    void writeToBatch(dao::VertexBatchBuilder& builder) const override {
        // 自定义渲染逻辑
        builder.addToBatch(someTexture);
    }
};
```

### 6. 上下文管理器 (Context)

`Context` 提供统一的状态和服务管理。

**状态管理（存储数据）：**
```cpp
// 定义状态类型
struct UserData {
    std::string name;
    int score;
};

// 注册状态
context.emplaceState<UserData>("张三", 100);

// 获取状态
auto& userData = context.getState<UserData>();
userData.score += 10;

// 检查状态是否存在
if (context.hasState<UserData>()) {
    // ...
}
```

**服务管理（共享功能）：**
```cpp
// 定义服务类型
class DatabaseService {
public:
    void connect() { /* ... */ }
    void query() { /* ... */ }
};

// 注册服务
context.emplaceService<DatabaseService>();

// 获取服务
auto& db = context.service<DatabaseService>();
db.connect();

// 注册应用指针（常用模式）
context.emplaceService<dao::App*>(&app);
```

---

## 完整示例解析

### 示例 1：计数器应用

```cpp
#include <daoLib.hpp>

// 计数器状态
struct CounterState {
    int count = 0;
};

class CounterPage : public dao::GeneralPage {
    dao::SimpleButton incrementBtn{100, 100, 200, 50, [this] {
        // 增加计数
        getContext().getState<CounterState>().count++;
    }};

    dao::SimpleButton decrementBtn{100, 200, 200, 50, [this] {
        // 减少计数
        getContext().getState<CounterState>().count--;
    }};

    dao::Text countText{100, 300, 32, dao::hexToRGBA("#000000")};

public:
    CounterPage() : GeneralPage("counter") {}

    std::vector<dao::i32> getRegisterTexture() const override {
        return {};
    }

    void init() override {
        // 初始化计数器状态
        if (!getContext().hasState<CounterState>()) {
            getContext().emplaceState<CounterState>();
        }
    }

    void update() override {
        // 更新计数显示
        int count = getContext().getState<CounterState>().count;
        countText.setContent(std::to_wstring(count));

        // 渲染组件
        addToBatch(incrementBtn, decrementBtn, countText);
    }

    void handleInputEvent(const SDL_Event& event) override {
        incrementBtn.handleEvent(event);
        decrementBtn.handleEvent(event);
    }
};

int main() {
    dao::App app{60};

    // 创建窗口和页面
    app.createWindow(400, 500, "counter")
        .addPage(std::make_unique<CounterPage>());

    app.run();
    return 0;
}
```

### 示例 2：用户登录界面

```cpp
#include <daoLib.hpp>
#include <string>

struct UserCredentials {
    std::u32string username;
    std::u32string password;
};

class LoginPage : public dao::GeneralPage {
    dao::InputBox usernameBox{100, 100, 300, 40};
    dao::InputBox passwordBox{100, 200, 300, 40};

    dao::SimpleButton loginBtn{100, 300, 150, 50, [this] {
        auto& creds = getContext().getState<UserCredentials>();
        creds.username = usernameBox.getText();
        creds.password = passwordBox.getText();

        // 这里添加登录验证逻辑
        std::cout << "登录尝试: "
                  << utf8::utf32to8(creds.username) << std::endl;
    }};

    dao::Text title{100, 50, 24, dao::hexToRGBA("#2c3e50"), U"用户登录"};
    dao::Text userLabel{50, 110, 18, dao::hexToRGBA("#34495e"), U"用户名:"};
    dao::Text passLabel{50, 210, 18, dao::hexToRGBA("#34495e"), U"密码:"};

public:
    LoginPage() : GeneralPage("login") {}

    std::vector<dao::i32> getRegisterTexture() const override {
        return {};
    }

    void init() override {
        if (!getContext().hasState<UserCredentials>()) {
            getContext().emplaceState<UserCredentials>();
        }
    }

    void update() override {
        addToBatch(usernameBox, passwordBox, loginBtn,
                   title, userLabel, passLabel);
    }

    void handleInputEvent(const SDL_Event& event) override {
        usernameBox.handleEvent(event);
        passwordBox.handleEvent(event);
        loginBtn.handleEvent(event);
    }
};

int main() {
    dao::App app{60};

    app.createWindow(500, 400, "login")
        .addPage(std::make_unique<LoginPage>());

    app.run();
    return 0;
}
```

---

## 高级功能

### 托盘系统

DaoLib 支持系统托盘图标和菜单。

**创建自定义托盘：**
```cpp
#include <core/frame/tray.hpp>

class MyTray : public dao::Tray {
public:
    MyTray(const std::string_view iconPath, const std::string_view tooltip)
        : Tray(iconPath, tooltip) {}

    void createTrayMenu() override {
        // 添加菜单项
        button("打开设置", [this](SDL_TrayEntry* entry) {
            getContext().service<dao::App*>()->showWindow("settings");
        });

        button("退出", [this](SDL_TrayEntry* entry) {
            getContext().service<dao::App*>()->close();
        });
    }
};

// 在主函数中使用
int main() {
    dao::App app{60};

    // 创建托盘
    app.createTray<MyTray>("icon.png", "我的应用")
        .createTrayMenu();

    // ... 其他代码
}
```

### 多窗口管理

```cpp
int main() {
    dao::App app{60};

    // 创建主窗口
    auto& mainWindow = app.createWindow(800, 600, "main", false, true);
    mainWindow.addPage(std::make_unique<MainPage>());

    // 创建设置窗口（初始隐藏）
    auto& settingsWindow = app.createWindow(400, 300, "settings", true);
    settingsWindow.addPage(std::make_unique<SettingsPage>());

    // 在需要时显示设置窗口
    // app.showWindow("settings");

    app.run();
    return 0;
}
```

### 纹理管理

**纹理预加载：**
```cpp
class MyPage : public dao::GeneralPage {
    static constexpr dao::i32 BUTTON_TEXTURE = 1;
    static constexpr dao::i32 BACKGROUND_TEXTURE = 2;

public:
    std::vector<dao::i32> getRegisterTexture() const override {
        return {BUTTON_TEXTURE, BACKGROUND_TEXTURE};
    }

    void update() override {
        // 使用纹理
        dao::Image bg{0, 0, 800, 600, BACKGROUND_TEXTURE};
        dao::Image btn{100, 100, 200, 100, BUTTON_TEXTURE};

        addToBatch(bg, btn);
    }
};
```

### 自定义 Drawable 组件

创建可重用的自定义 UI 组件：

```cpp
class ProgressBar : public dao::Drawable {
    dao::f32 x, y, width, height;
    dao::f32 progress; // 0.0 到 1.0
    dao::ColorRGBA color;

public:
    ProgressBar(dao::f32 x, dao::f32 y, dao::f32 width, dao::f32 height,
                dao::f32 progress, dao::ColorRGBA color)
        : x(x), y(y), width(width), height(height),
          progress(progress), color(color) {}

    void writeToBatch(dao::VertexBatchBuilder& builder) const override {
        // 绘制背景
        dao::Rectangle bg{x, y, width, height, dao::hexToRGBA("#ecf0f1")};
        builder.addToBatch(bg);

        // 绘制进度条
        dao::Rectangle bar{x, y, width * progress, height, color};
        builder.addToBatch(bar);

        // 绘制边框
        dao::Rectangle border{x, y, width, height, dao::hexToRGBA("#7f8c8d"), false};
        builder.addToBatch(border);
    }

    void setProgress(dao::f32 newProgress) {
        progress = std::clamp(newProgress, 0.0f, 1.0f);
    }
};

// 在页面中使用
ProgressBar progressBar{100, 100, 300, 20, 0.75f, dao::hexToRGBA("#3498db")};

void update() override {
    progressBar.setProgress(currentProgress);
    addToBatch(progressBar);
}
```

---

## 最佳实践

### 1. 资源管理

- **纹理预加载**：在 `getRegisterTexture()` 中声明所有需要的纹理
- **字体管理**：使用 `VertexBatchBuilder` 的字形图集
- **内存管理**：合理使用 `unique_ptr` 和 `shared_ptr`

### 2. 性能优化

- **批处理渲染**：使用 `addToBatch()` 集中渲染
- **避免每帧创建对象**：在页面生命周期中重用组件
- **合理使用事件处理**：只处理必要的事件

### 3. 代码组织

- **页面分离**：每个功能模块使用独立页面
- **组件封装**：将相关 UI 元素封装为自定义 Drawable
- **状态集中管理**：使用 Context 统一管理应用状态

### 4. 错误处理

```cpp
// 检查纹理是否加载
try {
    auto& texture = getTexture(textureId);
} catch (const std::exception& e) {
    std::cerr << "纹理加载失败: " << e.what() << std::endl;
}

// 检查状态是否存在
if (getContext().hasState<MyState>()) {
    // 安全访问状态
    auto& state = getContext().getState<MyState>();
}
```

---

## 常见问题

### Q1: 如何调试 UI 布局？

使用 `BoundingBox` 辅助调试：

```cpp
// 在组件周围绘制调试边框
dao::Rectangle debugRect{x, y, width, height, dao::hexToRGBA("#FF0000"), false};
addToBatch(debugRect);
```

### Q2: 如何实现页面切换？

```cpp
// 在页面中切换其他页面
getWindowController().switchTo("otherPage");

// 或使用窗口的切换方法
getWindow().switchPage("otherPageTitle");
```

### Q3: 如何处理中文输入？

DaoLib 内置支持 UTF-32 字符串，确保使用正确的字符串字面量：

```cpp
// 使用 UTF-32 字符串字面量 (C++11)
dao::Text text{100, 100, 24, dao::hexToRGBA("#000000"), U"中文文本"};

// 转换 std::string 到 UTF-32
std::string utf8Str = "中文";
std::u32string utf32Str = utf8::utf8to32(utf8Str);
```

### Q4: 如何自定义组件样式？

创建自定义按钮样式：

```cpp
class MyButtonStyle : public dao::ButtonStyle {
public:
    void apply(dao::Button& button, dao::ButtonStatus status) override {
        switch (status) {
            case dao::ButtonStatus::Normal:
                button.setBackgroundColor(dao::hexToRGBA("#3498db"));
                break;
            case dao::ButtonStatus::Hovered:
                button.setBackgroundColor(dao::hexToRGBA("#2980b9"));
                break;
            case dao::ButtonStatus::Pressed:
                button.setBackgroundColor(dao::hexToRGBA("#1c6ea4"));
                break;
            case dao::ButtonStatus::Disabled:
                button.setBackgroundColor(dao::hexToRGBA("#bdc3c7"));
                break;
        }
    }
};
```

---

## 故障排除

### 构建问题

1. **CMake 找不到 SDL3**：
   ```bash
   # 确保 vcpkg 工具链正确配置
   cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg根目录]/scripts/buildsystems/vcpkg.cmake
   ```

2. **C++23 特性不支持**：
   - 更新编译器版本
   - 检查 CMake 中的 `CMAKE_CXX_STANDARD` 设置

### 运行时问题

1. **窗口不显示**：
   - 检查窗口创建参数（特别是 `hidden` 参数）
   - 确保调用了 `app.run()`

2. **纹理不显示**：
   - 在 `getRegisterTexture()` 中正确注册纹理 ID
   - 检查纹理路径和格式

3. **输入事件不响应**：
   - 在 `handleInputEvent()` 中正确转发事件到组件
   - 检查事件类型和处理逻辑

---

## 下一步

- 查看 [API 文档](@ref daoLib.hpp) 获取详细类参考
- 参考测试示例了解更多用法
- 参与 DaoLib 开发，提交 Issue 和 Pull Request

---

*本教程基于 DaoLib 版本 0.0.1，最后更新于 2026年3月3日。*