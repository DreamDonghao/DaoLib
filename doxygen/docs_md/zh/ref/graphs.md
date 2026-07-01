@page graphs 图形组件参考 {#graphs}

[TOC]

# 图形组件参考 — IGraph / Circle / Ellipse / Rectangle / Triangle / Line / Arc / Sector / Ring / RoundedRectangle / Polygon

---

## IGraph — 图形接口 {#igraph}

所有几何图形的抽象基类，继承自 `IDrawable`。

```cpp
#include <components/graphs/IGraph.hpp>

namespace dao::ifc {
    class IGraph : public IDrawable {
    public:
        virtual void translate(f32 dx, f32 dy) = 0;
        virtual void rotate(f32 cx, f32 cy, f32 theta) = 0;
        void writeToBatch(BatchRenderer &batchRenderer) const override = 0;
    };
}
```

| 方法 | 说明 |
|------|------|
| `translate(dx, dy)` | 平移，`dx>0` 向右，`dy>0` 向下 |
| `rotate(cx, cy, theta)` | 绕 `(cx, cy)` 旋转 `theta` 弧度 |
| `writeToBatch(batch)` | 将顶点写入批处理渲染器 |

所有 setter（`setPosition`、`setRadius`、`setColor` 等）都会自动调用 `updateVertices()` 重建顶点缓冲区。

---

## Circle — 圆形 {#graph-circle}

@ref graphs "→ 返回教程：几何图形"

### 头文件

```cpp
#include <components/graphs/Circle.hpp>
```

### 构造函数

```cpp
Circle(f32 cx, f32 cy, f32 radius,
       const ColorRGBA &color = White, i32 segments = 32);
```

以圆心为共同顶点、边缘点为底的 triangle-fan 渲染。

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(cx, cy)` | 设置圆心 |
| `setRadius(radius)` | 设置半径（自动重建顶点） |
| `setColor(color)` | 设置填充颜色 |
| `setSegments(n)` | 设置分段数（越大越平滑） |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Ellipse — 椭圆 {#graph-ellipse}

### 头文件

```cpp
#include <components/graphs/Ellipse.hpp>
```

### 构造函数

```cpp
Ellipse(f32 cx, f32 cy, f32 radiusX, f32 radiusY,
        const ColorRGBA &color = White, i32 segments = 32);
```

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(cx, cy)` | 设置中心点 |
| `setRadius(rx, ry)` | 设置 X/Y 方向半径 |
| `setColor(color)` | 设置填充颜色 |
| `setSegments(n)` | 设置分段数 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Rectangle — 矩形 {#graph-rectangle}

### 头文件

```cpp
#include <components/graphs/Rectangle.hpp>
```

### 构造函数

```cpp
Rectangle(f32 x = 0, f32 y = 0, f32 width = 0, f32 height = 0,
          const ColorRGBA &color = White);
```

轴对齐矩形，由左上角坐标和宽高定义，2 个三角形渲染。

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(x, y)` | 设置左上角位置 |
| `setSize(w, h)` | 设置尺寸 |
| `setColor(color)` | 设置填充颜色 |
| `x() / y()` | 获取左上角坐标 |
| `width() / height()` | 获取尺寸 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Triangle — 三角形 {#graph-triangle}

### 头文件

```cpp
#include <components/graphs/Triangle.hpp>
```

### 构造函数

```cpp
Triangle(f32 x1, f32 y1, f32 x2, f32 y2,
         f32 x3, f32 y3, const ColorRGBA &color = White);
```

### 方法

| 方法 | 说明 |
|------|------|
| `setVertex(index, x, y)` | 设置指定顶点（index: 0/1/2） |
| `setColor(color)` | 设置填充颜色 |
| `vertexX(i) / vertexY(i)` | 获取顶点坐标 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Line — 线段 {#graph-line}

### 头文件

```cpp
#include <components/graphs/Line.hpp>
```

### 构造函数

```cpp
Line(f32 x1, f32 y1, f32 x2, f32 y2,
     f32 thickness = 1.0f, const ColorRGBA &color = White);
```

由起点和终点定义，垂直方向扩展厚度生成四边形。

### 方法

| 方法 | 说明 |
|------|------|
| `setStart(x, y)` | 设置起点 |
| `setEnd(x, y)` | 设置终点 |
| `setThickness(t)` | 设置厚度 |
| `setColor(color)` | 设置颜色 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Arc — 圆弧 {#graph-arc}

### 头文件

```cpp
#include <components/graphs/Arc.hpp>
```

### 构造函数

```cpp
Arc(f32 cx, f32 cy, f32 radius,
    f32 startAngle, f32 endAngle,
    f32 thickness = 1.0f, const ColorRGBA &color = White,
    i32 segments = 32);
```

从起始角度到结束角度的圆弧，通过将弧线扩展厚度生成内外圈顶点，以三角形条带渲染。

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(cx, cy)` | 设置圆心 |
| `setRadius(r)` | 设置半径 |
| `setAngles(start, end)` | 设置起止角度（弧度） |
| `setThickness(t)` | 设置厚度 |
| `setColor(color)` | 设置颜色 |
| `setSegments(n)` | 设置分段数 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Sector — 扇形 {#graph-sector}

### 头文件

```cpp
#include <components/graphs/Sector.hpp>
```

### 构造函数

```cpp
Sector(f32 cx, f32 cy, f32 radius,
       f32 startAngle, f32 endAngle,
       const ColorRGBA &color = White, i32 segments = 32);
```

以圆心为共同顶点、弧线边缘点为底的 triangle-fan 渲染。

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(cx, cy)` | 设置圆心 |
| `setRadius(r)` | 设置半径 |
| `setAngles(start, end)` | 设置起止角度（弧度） |
| `setColor(color)` | 设置颜色 |
| `setSegments(n)` | 设置分段数 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Ring — 圆环 {#graph-ring}

### 头文件

```cpp
#include <components/graphs/Ring.hpp>
```

### 构造函数

```cpp
Ring(f32 cx, f32 cy, f32 innerRadius, f32 outerRadius,
     const ColorRGBA &color = White, i32 segments = 32);
```

由圆心、内外半径定义的圆环，以分段四边形条带渲染。

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(cx, cy)` | 设置圆心 |
| `setInnerRadius(r)` | 设置内半径 |
| `setOuterRadius(r)` | 设置外半径 |
| `setColor(color)` | 设置颜色 |
| `setSegments(n)` | 设置分段数 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## RoundedRectangle — 圆角矩形 {#graph-roundedrect}

### 头文件

```cpp
#include <components/graphs/RoundedRectangle.hpp>
```

### 构造函数

```cpp
RoundedRectangle(f32 x, f32 y, f32 width, f32 height,
                 f32 cornerRadius, const ColorRGBA &color = White,
                 i32 cornerSegments = 8);
```

以中心点为共同顶点的 triangle-fan 渲染，四个角分别用分段弧线逼近。

### 方法

| 方法 | 说明 |
|------|------|
| `setPosition(x, y)` | 设置左上角位置 |
| `setSize(w, h)` | 设置尺寸 |
| `setCornerRadius(r)` | 设置圆角半径（自动限制 ≤ min(w,h)/2） |
| `setColor(color)` | 设置填充颜色 |
| `setCornerSegments(n)` | 设置每个角的分段数 |
| `translate(dx, dy)` | 平移 |
| `rotate(cx, cy, theta)` | 旋转 |

---

## Polygon\<N\> — 多边形（模板） {#graph-polygon}

### 头文件

```cpp
#include <components/graphs/Polygon.hpp>
```

### 构造函数

```cpp
template<i32 VertexNum>
template<std::same_as<Vertex>... Args>
explicit Polygon(Args &&... args);
```

编译期固定顶点数的多边形。`VertexNum` 必须 > 2，传入的 `Vertex` 数量必须与 `VertexNum` 一致。

### 方法

| 方法 | 说明 |
|------|------|
| `translate(dx, dy)` | 平移所有顶点 |
| `rotate(cx, cy, theta)` | 旋转所有顶点 |
| `writeToBatch(batch)` | 以 triangle-fan 写入渲染器 |

### 示例

```cpp
// 五边形
dao::Polygon<5> pentagon{
    dao::Vertex{400, 200, dao::Red},
    dao::Vertex{500, 270, dao::Red},
    dao::Vertex{460, 370, dao::Red},
    dao::Vertex{340, 370, dao::Red},
    dao::Vertex{300, 270, dao::Red}
};
```