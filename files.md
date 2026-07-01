# DaoLib 项目结构介绍

本文档描述 DaoLib 项目的文件结构，帮助开发者快速了解项目布局及各目录的用途。

## 概述

DaoLib 是一个基于 SDL3 的跨平台 C++ GUI 开发框架，项目采用 CMake 构建，包含核心库、命令行工具、示例项目和测试等模块。

## 根目录结构

```
├── CMakeLists.txt          # 根 CMake 配置文件
├── README.md               # 项目说明
├── TODO.md                 # 待办事项
├── vcpkg.json              # vcpkg 依赖清单
├── FILES.md                # 项目文件结构说明（本文件）
├── releases/               # 发布文件存放目录
├── .github/                # GitHub 工作流配置
├── libCode/                # 核心库源代码
├── test/                   # 测试项目
├── cli_tool/               # 命令行工具（Go 实现）
├── example/                # 示例项目
├── docs/                   # 项目文档
├── doxygen/                # Doxygen 配置与生成文档
└── doxygen_output/         # Doxygen 输出目录（生成）
```

## 核心库 (libCode/)

核心库提供了 DaoLib 框架的所有功能，采用头文件 (.hpp) 与源文件 (.cpp) 分离的典型 C++ 项目结构。

### include/ — 头文件目录

```
include/
├── daolib.hpp                        # 聚合头文件（包含所有公共接口）
├── components/                       # UI 组件
│   ├── graphs.hpp                    # 图形组件聚合头文件
│   ├── Image.hpp                     # 图像组件
│   ├── Text.hpp                      # 文本渲染组件
│   ├── TextBox.hpp                   # 文本框组件（支持自动换行和区域裁剪）
│   ├── VoidPage.hpp                  # 空页面组件
│   ├── controls/                     # 控件组件
│   │   ├── IButton.hpp               # 按钮接口
│   │   ├── InputBox.hpp              # 输入框组件
│   │   └── SimpleButton.hpp          # 简单按钮组件
│   └── graphs/                       # 几何图形组件
│       ├── IGraph.hpp                # 图形接口（平移、旋转、渲染）
│       ├── Arc.hpp                   # 弧形图形
│       ├── Circle.hpp                # 圆形图形
│       ├── Ellipse.hpp               # 椭圆图形
│       ├── Line.hpp                  # 线段图形
│       ├── Polygon.hpp               # 多边形图形（模板，编译期顶点数）
│       ├── Rectangle.hpp             # 矩形图形
│       ├── Ring.hpp                  # 圆环图形
│       ├── RoundedRectangle.hpp      # 圆角矩形图形
│       ├── Sector.hpp                # 扇形图形
│       └── Triangle.hpp              # 三角形图形
├── core/                             # 核心模块
│   ├── frame/                        # 框架层（应用、窗口、上下文等）
│   │   ├── App.hpp                   # 应用程序入口
│   │   ├── AppController.hpp         # 应用级控制器
│   │   ├── Context.hpp               # 上下文管理
│   │   ├── FrameLimiter.hpp          # 帧率限制器
│   │   ├── IPage.hpp                 # 页面接口
│   │   ├── Tray.hpp                  # 系统托盘
│   │   ├── Window.hpp                # 窗口管理
│   │   └── WindowController.hpp      # 窗口级控制器
│   ├── render/                       # 渲染模块
│   │   ├── BatchRenderer.hpp         # 批处理渲染器
│   │   ├── IDrawable.hpp             # 可绘制接口
│   │   └── primitives/               # 基础图元
│   │       ├── AtlasRegion.hpp       # 纹理图集区域
│   │       ├── ColorRGB.hpp          # RGB 颜色定义
│   │       ├── GlyphAtlas.hpp        # 字形图集
│   │       └── Vertex.hpp            # 顶点数据结构
│   └── tool/                         # 工具类
│       ├── better_stl.hpp            # STL 增强工具
│       ├── BoundingBox.hpp           # 包围盒计算
│       ├── Log.hpp                   # 日志工具
│       ├── MappedRegion.hpp          # 映射区域
│       ├── Point.hpp                 # 点与几何计算
│       ├── RingVector.hpp            # 环形向量容器
│       ├── TickTimer.hpp             # 计时器
│       └── type.hpp                  # 类型别名（i32, f32, utf32str 等）
├── interface/                        # 接口定义
│   ├── IGeneralPage.hpp              # 通用页面接口
│   └── IButtonStyle.hpp              # 按钮样式接口
├── web/                              # Web 模块
│   └── http/                         # HTTP 客户端
│       ├── Headers.hpp               # HTTP 请求/响应头
│       ├── HttpClient.hpp            # HTTP 客户端
│       ├── HttpPoller.hpp            # HTTP 轮询器
│       └── HttpTypes.hpp             # HTTP 类型定义
└── database/                         # 数据库模块
    └── sqlite/
        └── Sqlite.hpp                # SQLite 数据库封装
```

### src/ — 源文件目录

```
src/
├── daolib.cpp                        # 聚合编译单元
├── components/
│   └── graphs/                       # 图形组件实现
│       ├── Arc.cpp
│       ├── Circle.cpp
│       ├── Ellipse.cpp
│       ├── Line.cpp
│       ├── Rectangle.cpp
│       ├── Ring.cpp
│       ├── RoundedRectangle.cpp
│       ├── Sector.cpp
│       └── Triangle.cpp
├── core/
│   ├── frame/
│   │   ├── App.cpp
│   │   ├── AppController.cpp
│   │   ├── Context.cpp
│   │   ├── Window.cpp
│   │   └── WindowController.cpp
│   ├── render/
│   │   ├── BatchRenderer.cpp
│   │   ├── Texture.cpp
│   │   └── primitives/
│   │       └── GlyphAtlas.cpp
│   └── tool/
│       ├── BoundingBox.cpp
│       └── MappedRegion.cpp
├── interface/
│   └── IGeneralPage.cpp
└── web/
    └── http/
        ├── Headers.cpp
        ├── HttpClient.cpp
        ├── HttpPoller.cpp
        └── HttpTypes.cpp
```

## 测试项目 (test/)

图形组件测试页面，支持可视化验证所有图形组件的渲染与动画效果。

```
test/
├── CMakeLists.txt               # 测试项目构建配置
├── main.cpp                     # 测试程序入口
├── assets/                      # 测试资源
│   ├── ttf/                     # 字体文件
│   └── textures/
│       ├── atlas/               # 生成的纹理图集
│       ├── config/              # 图集配置
│       │   └── atlas_region.cpp
│       ├── inc/                 # 纹理 ID 头文件
│       │   └── texture_id.hpp
│       └── input_images/        # 原始输入图片
└── pages/                       # 测试页面
    ├── graph_test_page.hpp      # 图形组件测试页面
    └── graph_test_page.cpp      # 测试页面实现
```

测试页面功能：
- 3×3 网格展示 9 种几何图形
- 点击/按键(1-9)选择图形，滚轮/方向键切换测试用例
- 按 A 开关独立动画（各图形效果不同：半径脉动、旋转、颜色循环等）
- 左上角实时 FPS 显示

## 命令行工具 (cli_tool/)

Go 实现的命令行工具，提供项目创建、资源打包等功能。

```
cli_tool/
├── main.go                     # 主程序入口
├── go.mod / go.sum             # Go 模块与依赖
├── COMMANDS.md                 # 命令说明
├── atlas/                      # 纹理打包工具
├── cmdRun/                     # 命令运行 / 模板
└── util/                       # 工具函数
```

## 示例项目 (example/)

```
example/
├── main.cpp                    # 示例入口
├── CMakeLists.txt              # 示例构建配置
├── vcpkg.json                  # 依赖配置
├── assets/                     # 示例资源
│   └── textures/
│       ├── atlas/ / config/ / inc/   # 纹理图集、配置与 ID
│       └── input_images/             # 原始图片
├── pages/                      # 示例页面
└── tray/                       # 托盘示例
```

## 文档

- **docs/** — `en/` 英文文档，`zh/` 中文文档
- **doxygen/** — Doxygen 配置（`Doxyfiles/`）、Markdown 源（`docs_md/`）、样式（`style/`）
- **doxygen_output/** — 生成的 HTML 文档（不提交到版本控制）

## 构建

```sh
cmake -B cmake-build-debug -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
cmake --build cmake-build-debug
```

- C++ 标准：C++23
- 模板类和纯虚接口保持头文件声明；非模板类将实现提取到 .cpp 文件
- 批处理渲染器使用 UNITY_BUILD 编译模式
- 生成目录（`cmake-build-debug/`、`doxygen_output/`）不提交到版本控制

---

*本文档基于项目当前结构编写，随项目发展可能会有变化。*