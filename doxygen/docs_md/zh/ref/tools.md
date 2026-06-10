@page tools 工具模块参考 {#tools}

[TOC]

# 工具模块参考 — Log / BoundingBox / 类型别名

---

## Log — 日志系统 {#logging}

`dao::Log` 提供分级、带时间戳和源码位置的彩色日志输出。

@ref logging "→ 返回教程：日志系统"

### 头文件

```cpp
#include <core/tool/Log.hpp>
```

### LogLevel 枚举

| 级别 | 用途 |
|------|------|
| `TRACE` | 最细粒度，程序每一步执行 |
| `DEBUG` | 调试信息，运行状态 |
| `INFO` | 常规信息，重要事件 |
| `WARN` | 警告，可能有问题 |
| `ERROR` | 错误，可继续运行 |
| `FATAL` | 致命错误，即将退出 |

### 基本用法

```cpp
// 普通输出（自动附加时间戳和源码位置）
dao::Log{dao::LogLevel::INFO}("服务器启动成功");
dao::Log{dao::LogLevel::WARN}("内存使用: {}%", 85);
dao::Log{dao::LogLevel::ERROR}("连接失败: {}", errMsg);

// fmt 格式化
dao::Log{dao::LogLevel::DEBUG}.fmt("变量 x = {}, y = {}", x, y);
```

### 彩色输出

```cpp
// 开启 ANSI 彩色输出
dao::Log::openStyleOutPut();

// 关闭彩色输出
dao::Log::closeStyleOutPut();
```

### DAO_ERROR_LOG 宏

```cpp
DAO_ERROR_LOG("纹理加载失败: " + filePath);
// 输出:
// [ERROR] 2026-06-14 10:30:45.123:
// 纹理加载失败: path/to/file.png
// 文件[File]: page.cpp
// 行数[Line]: 42
// 函数[Function]: MyPage::open
```

---

## BoundingBox — 边界框 {#boundingbox}

`BoundingBox` 表示一个轴对齐的矩形范围，用于碰撞检测和区域计算。

### 头文件

```cpp
#include <core/tool/BoundingBox.hpp>
```

### 构造函数

```cpp
BoundingBox();                                          // (0,0,0,0)
BoundingBox(f32 left, f32 top, f32 right, f32 bottom);  // 指定四边
```

### 方法

| 方法 | 说明 |
|------|------|
| `set(l, t, r, b)` | 设置四边 |
| `setLeft(l) / setTop(t) / setRight(r) / setBottom(b)` | 设置单边 |
| `getLeft() / getTop() / getRight() / getBottom()` | 获取边坐标 |
| `getWidth() / getHeight()` | 获取尺寸 |
| `moveHorizontal(d)` | 水平移动（正右负左） |
| `moveVertical(d)` | 竖直移动（正上负下） |
| `normalize()` | 修复颠倒的边（left>right 或 top>bottom） |
| `isValid()` | 是否有效（left≤right 且 top≤bottom） |
| `isInBoundingBox(x, y)` | 点是否在内部 |
| `isIntersects(other)` | 是否与另一个边界框相交 |

---

## 类型别名 {#types}

### 头文件

```cpp
#include <core/tool/type.hpp>
```

### 整数类型

| 别名 | 实际类型 |
|------|---------|
| `i8` | `int8_t` |
| `i16` | `int16_t` |
| `i32` | `int32_t` |
| `i64` | `int64_t` |
| `u8` | `uint8_t` |
| `u16` | `uint16_t` |
| `u32` | `uint32_t` |
| `u64` | `uint64_t` |

### 浮点类型

| 别名 | 实际类型 |
|------|---------|
| `f32` | `float` |
| `f64` | `double` |

### 字符串

| 别名 | 实际类型 |
|------|---------|
| `utf32char` | `char32_t` |
| `utf32str` | `std::u32string` |
| `utf32str_view` | `std::u32string_view` |

### 容器

| 别名 | 实际类型 |
|------|---------|
| `hash_map<K,V>` | `std::unordered_map<K,V>` |

### 其他

| 别名 | 实际类型 |
|------|---------|
| `json` | `nlohmann::json` |

### 使用建议

DaoLib 代码统一使用这些别名而非标准类型名，保持代码一致性。