@page components UI 组件参考 {#components}

[TOC]

# UI 组件参考 — Text / TextBox / Button

---

## Text — 文本渲染 {#text-component}

`dao::Text` 在指定位置渲染 UTF-32 文本，不处理区域裁剪和自动换行。如需这些功能，请使用 @ref textbox-component "TextBox"。

@ref text "→ 返回教程：文本渲染"

### 头文件

```cpp
#include <components/Text.hpp>
```

### 构造函数

```cpp
Text(f32 x, f32 y, f32 lineHeight,
     const ColorRGBA &color = Black, utf32str str = U"");
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `x, y` | `f32` | 起始坐标（左上角） |
| `lineHeight` | `f32` | 行高（像素） |
| `color` | `ColorRGBA` | 文本颜色，默认黑色 |
| `str` | `utf32str` | UTF-32 文本内容 |

### 方法

| 方法 | 说明 |
|------|------|
| `setContent(str)` | 设置文本内容 |
| `getContent()` | 获取文本内容 |
| `setPosition(x, y)` | 设置位置 |
| `x() / y()` | 获取坐标 |
| `setLineHeight(h)` | 设置行高 |
| `lineHeight()` | 获取行高 |
| `setColor(color)` | 设置颜色 |
| `writeToBatch(batch)` | 写入渲染批次 |

### 支持换行符

文本中的 `\n` 会触发换行：

```cpp
text.setContent(U"第一行\n第二行\n第三行");
```

---

## TextBox — 文本框 {#textbox-component}

`TextBox` 在指定矩形区域内渲染文本，支持自动换行和边界裁剪。

@ref text "→ 返回教程：文本渲染"

### 头文件

```cpp
#include <components/TextBox.hpp>
```

### 构造函数

```cpp
TextBox(f32 x, f32 y, f32 width, f32 height,
        f32 lineHeight, const ColorRGBA &color = White);
```

### 方法

| 方法 | 说明 |
|------|------|
| `setContent(str)` | 设置文本内容 |
| `getContent()` | 获取文本内容 |
| `setPosition(x, y)` | 设置位置 |
| `setSize(w, h)` | 设置尺寸 |
| `getBounds()` | 获取边界框 |
| `setLineHeight(h)` | 设置行高 |
| `lineHeight()` | 获取行高 |
| `setColor(color)` | 设置颜色 |
| `setWordWrap(bool)` | 开启/关闭自动换行（默认开启） |
| `wordWrap()` | 获取自动换行状态 |
| `setTextAlignToEnd(bool)` | 末尾优先显示（默认 false，开头优先） |
| `textAlignToEnd()` | 获取对齐模式 |
| `writeToBatch(batch)` | 写入渲染批次 |

### 自动换行

当 `setWordWrap(true)`（默认），文本超出右边界自动换到下一行。超出底部边界的文本被裁剪。

### 对齐模式

- `setTextAlignToEnd(false)`（默认）：从顶部向下渲染
- `setTextAlignToEnd(true)`：从底部向上渲染（类似聊天记录自动滚动到底部）

---

## Button — 按钮 {#button}

`dao::Button` 是一个简单的矩形按钮组件，实现 `IButton` 接口。

@ref controls "→ 返回教程：按钮与输入"

### 头文件

```cpp
#include <components/controls/SimpleButton.hpp>
```

### 构造函数

```cpp
Button(f32 x, f32 y, f32 width, f32 height,
       std::function<void()> onClick,
       ButtonStatus buttonStatus = ButtonStatus::Normal);
```

| 参数 | 类型 | 说明 |
|------|------|------|
| `x, y` | `f32` | 左上角坐标 |
| `width, height` | `f32` | 按钮尺寸 |
| `onClick` | `function<void()>` | 点击回调 |
| `buttonStatus` | `ButtonStatus` | 初始状态 |

### ButtonStatus 枚举

| 值 | 说明 |
|----|------|
| `Normal` | 正常状态 |
| `Hover` | 鼠标悬停 |
| `Pressed` | 鼠标按下 |
| `Disabled` | 禁用（不响应事件） |

### 方法

| 方法 | 说明 |
|------|------|
| `handleEvent(event)` | 处理 SDL 事件（必须在页面 `handleInputEvent` 中调用） |
| `isEnable()` | 按钮是否可用 |
| `getStatus()` | 获取当前状态 |
| `setStatus(status)` | 设置状态 |
| `getBoundingBox()` | 获取边界框 |
| `writeToBatch(batch)` | 渲染按钮（当前为绿色矩形占位） |

### 状态机

```
Normal ──鼠标进入──→ Hover ──左键按下──→ Pressed ──左键释放──→ 触发 onClick → Hover
  ↑                     │                     │
  └───鼠标离开──────────┴─────────────────────┘
```

### 使用示例

```cpp
class MyPage : public dao::ifc::IGeneralPage {
    dao::Button m_btn{100, 100, 200, 50, [this] {
        dao::Log{dao::LogLevel::INFO}("按钮被点击");
    }};

public:
    void update() override {
        addToBatch(m_btn);
    }

    void handleInputEvent(const SDL_Event &event) override {
        m_btn.handleEvent(event);  // 必须调用
    }
};
```