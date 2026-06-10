@page render 渲染系统参考 {#render}

[TOC]

# 渲染系统参考 — BatchRenderer / ColorRGBA / Vertex

---

## BatchRenderer — 批处理渲染器 {#batchrenderer}

`BatchRenderer` 负责将零散的渲染图元按纹理图集分组，合并为批次提交 GPU，减少 Draw Call。

@ref quickstart "→ 返回教程"

### 头文件

```cpp
#include <core/render/BatchRenderer.hpp>
```

### 构造函数

```cpp
explicit BatchRenderer(i32 verticesCount = 1024);
```

| 参数 | 说明 |
|------|------|
| `verticesCount` | 顶点池默认最大容量 |

### 核心方法

#### init

```cpp
void init(SDL_Renderer *renderer);
```

初始化渲染器，创建默认白色纹理和字形图集纹理。由 Window 内部调用。

#### clear

```cpp
void clear();
```

清理本帧的批处理数据。应在每帧开始时调用（IGeneralPage 的 `update()` 中已自动处理）。

#### loadAtlas

```cpp
void loadAtlas(i32 textureId);
```

根据纹理 ID 加载对应的纹理图集到 GPU（幂等，已加载则跳过）。

#### render

```cpp
void render();
```

执行渲染：清空屏幕 → 按纹理分组绘制所有批次 → 呈现到屏幕。

#### allocateVertices

```cpp
SDL_Vertex *allocateVertices(i32 atlasID, i32 count);
```

从顶点池中分配 `count` 个顶点空间，返回写入指针。组件在 `writeToBatch()` 中调用此方法。

#### loadGlyph

```cpp
void loadGlyph(utf32char charCode);
```

按需加载指定字符的字形到图集。

#### getGlyphAtlas

```cpp
const GlyphAtlas &getGlyphAtlas();
```

获取字形图集引用。

### 渲染流程

```
每帧:
  clear()                    → 清空批数据
  page->update()             → addToBatch(comp1, comp2, ...)
    comp->writeToBatch(batch)→ 组件分配顶点 + 写入数据
  render()                   → 按纹理分组绘制 + 呈现
```

---

## ColorRGBA — 颜色 {#colors}

`ColorRGBA` 表示一个 32 位 RGBA 颜色值，RGB 通道 0-255，Alpha 通道 0.0-1.0。

@ref colors "→ 返回教程：颜色系统"

### 头文件

```cpp
#include <core/render/primitives/ColorRGB.hpp>
```

### 构造函数

```cpp
// RGBA 分量构造
constexpr ColorRGBA(i32 r = 0, i32 g = 0, i32 b = 0, f32 a = 1.0f);

// 十六进制字符串构造
explicit ColorRGBA(std::string_view hex);
```

### 预定义颜色常量

| 常量 | 值 |
|------|-----|
| `Black` | `#000000` |
| `White` | `#FFFFFF` |
| `Red` | `#FF0000` |
| `Green` | `#00FF00` |
| `Blue` | `#0000FF` |
| `Yellow` | `#FFFF00` |
| `Cyan` | `#00FFFF` |
| `Magenta` | `#FF00FF` |
| `Transparent` | `#00000000` |
| `NullRGBA` | `(-1,-1,-1,-1)` 无效哨兵值 |

### 静态方法

```cpp
static constexpr ColorRGBA hexToRGBA(std::string_view hex) noexcept;
```

支持格式：`#RRGGBB`、`RRGGBB`、`#RRGGBBAA`、`RRGGBBAA`。无效格式返回白色。

### 方法

| 方法 | 说明 |
|------|------|
| `set(r, g, b, a)` | 设置全部通道 |
| `set(hex)` | 通过十六进制字符串设置 |
| `setR(r) / setG(g) / setB(b)` | 设置单通道 |
| `setA(a)` | 设置透明度 |
| `r() / g() / b()` | 获取 RGB 通道（0-255） |
| `a()` | 获取透明度（0.0-1.0） |
| `isValid()` | 检查是否为有效颜色 |
| `isNull()` | 检查是否为 NullRGBA |
| `getSDLFColor()` | 转换为 SDL_FColor |

### 比较

```cpp
if (color1 == color2) { /* ... */ }
if (color1 != color2) { /* ... */ }
```

---

## Vertex — 顶点 {#vertex}

单个渲染顶点，包含位置、颜色和纹理坐标。

### 头文件

```cpp
#include <core/render/primitives/Vertex.hpp>
```

### 构造函数

```cpp
Vertex(f32 x = 0, f32 y = 0, const ColorRGBA &color = White,
       f32 u = 0, f32 v = 0);
```

| 参数 | 说明 |
|------|------|
| `x, y` | 屏幕坐标 |
| `color` | 顶点颜色 |
| `u, v` | 纹理坐标（0.0-1.0） |

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(x, y)` | 设置位置 |
| `setColor(color)` | 设置颜色 |
| `setUV(u, v)` | 设置纹理坐标 |
| `x() / y()` | 获取坐标 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 绕点旋转 |
| `getSDLVertex()` | 转换为 SDL_Vertex |