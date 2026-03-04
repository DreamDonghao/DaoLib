# DaoUI 项目结构介绍

本文档描述 DaoUI 项目的文件结构，帮助开发者快速了解项目布局及各目录的用途。

## 概述

DaoUI 是一个基于 SDL3 的跨平台 C++ GUI 开发框架，项目采用 CMake 构建，包含核心库、命令行工具、示例项目和测试等模块。

## 根目录结构

```
├── CMakeLists.txt          # 根 CMake 配置文件
├── README.md               # 英文项目说明
├── README.zh.md            # 中文项目说明
├── daoConfig.cmake         # CMake 包配置文件
├── vcpkg.json              # vcpkg 依赖清单
├── releases/               # 发布文件存放目录
├── .git/                   # Git 版本控制数据
├── .github/                # GitHub 工作流配置
├── code/                   # 核心库源代码
├── test/                   # 测试项目
├── cli_tool/               # 命令行工具（Go 实现）
├── example/                # 示例项目
├── docs/                   # 项目文档
├── doxygen/                # Doxygen 配置与生成文档
├── doxygen_output/         # Doxygen 输出目录（生成）
```

## 核心库 (code/)

核心库提供了 DaoUI 框架的所有功能，采用头文件与源文件分离的典型 C++ 项目结构。

### include/ – 头文件目录

```
include/
├── daoui.hpp                  # 主入口头文件（包含所有公共接口）
├── component/                 # UI 组件
│   ├── animated_sprite.hpp    # 动画精灵组件
│   ├── button/                # 按钮相关组件
│   │   ├── simple_button.hpp
│   │   ├── simple_texture_button_style.hpp
│   │   └── simple_text_button_style.hpp
│   ├── image.hpp              # 图像组件
│   ├── input/                 # 输入组件
│   │   ├── input_box.hpp
│   │   └── input_box_style.hpp
│   └── rectangle.hpp          # 矩形组件
├── core/                      # 核心模块
│   ├── frame/                 # 框架层（应用、窗口、上下文等）
│   │   ├── app.hpp
│   │   ├── app_controller.hpp
│   │   ├── context.hpp
│   │   ├── frame_limiter.hpp
│   │   ├── tray.hpp
│   │   ├── window.hpp
│   │   └── window_controller.hpp
│   ├── render/                # 渲染模块
│   │   ├── atlas_texture.hpp
│   │   ├── primitives/        # 基础图元
│   │   │   ├── atlas_region.hpp
│   │   │   ├── color_rgb.hpp
│   │   │   ├── glyph_atlas.hpp
│   │   │   ├── Vertex.hpp
│   │   │   └── triangle.hpp
│   │   ├── text.hpp
│   │   └── vertex_batch_builder.hpp
│   └── tool/                  # 工具类
│       ├── better_stl.hpp
│       ├── bounding_box.hpp
│       ├── mapped_region.hpp
│       ├── ring_vector.hpp
│       └── tick_timer.hpp
└── interface/                 # 接口定义
    ├── button.hpp
    ├── drawable.hpp
    └── page.hpp
```

### src/ – 源文件目录

源文件目录结构与头文件对应，提供各模块的具体实现。

```
src/
├── daoui.cpp                     # 库实现入口
├── component/
│   ├── button/
│   │   └── simple_button.cpp
│   └── image.cpp
├── core/
│   ├── frame/
│   │   ├── app.cpp
│   │   ├── app_controller.cpp
│   │   ├── context.cpp
│   │   ├── window.cpp
│   │   └── window_controller.cpp
│   ├── render/
│   │   ├── atlas_texture.cpp
│   │   ├── primitives/
│   │   │   └── glyph_atlas.hpp.cpp
│   │   └── vertex_batch_builder.cpp
│   └── tool/
│       ├── bounding_box.cpp
│       └── mapped_region.cpp
└── interface/
    └── general_page.cpp
```

## 测试项目 (test/)

用于验证框架功能的测试应用程序，结构类似于用户项目。

```
test/
├── main.cpp                     # 测试程序入口
├── assets/                      # 测试资源
│   └── textures/
│       ├── config/
│       │   └── atlas_region.cpp
│       ├── inc/
│       │   └── texture_id.hpp
│       ├── atlas/              # 生成的纹理图集
│       ├── config/             # 图集配置
│       ├── inc/                # 纹理ID头文件
│       └── input_images/       # 原始输入图片
├── pages/                       # 测试页面
│   ├── hello_dao_page.cpp
│   └── hello_dao_page.hpp
└── tray/                        # 托盘测试
    └── my_tray.hpp
```

## 命令行工具 (cli_tool/)

用 Go 实现的命令行工具，提供项目创建、资源打包、安装等功能。

```
cli_tool/
├── main.go                     # 主程序入口
├── go.mod                      # Go 模块定义
├── go.sum                      # 依赖校验
├── atlas/                      # 纹理打包工具
├── cmdRun/                     # 命令运行相关
├── util/                       # 工具函数
└── build/                      # 构建输出（生成）
```

## 示例项目 (example/)

展示如何使用 DaoUI 框架的示例程序。

```
example/
├── main.cpp                    # 示例入口
├── CMakeLists.txt              # 示例项目构建配置
├── vcpkg.json                  # 依赖配置
├── assets/                     # 示例资源
├── context/                    # 上下文配置
├── pages/                      # 示例页面
├── tray/                       # 托盘示例
└── cmake-build-debug/          # 构建目录（生成）
```

## 文档与生成文件

- **docs/** – 项目文档目录
  - `en/` – 英文文档
  - `zh/` – 中文文档
- **doxygen/** – Doxygen 相关文件
  - `Doxyfiles/` – Doxygen 配置文件
  - `docs_md/` – 用于生成文档的 Markdown 源文件
  - `style/` – 文档样式文件
- **doxygen_output/** – Doxygen 生成的 HTML 文档（运行后产生）
- **build/** – CMake 构建过程中的临时文件
- **out/** – 编译输出的二进制文件
- **releases/** – 发布版本打包文件

## 构建与使用

### 快速开始

1. 安装依赖：CMake、vcpkg
2. 使用 `cli_tool` 中的 `dao` 命令创建新项目
3. 将图片资源放入 `assets/textures/input_images/` 后运行 `dao texture pack` 打包
4. 使用 CMake 构建并运行

### 项目约定

- 公共头文件放在 `code/include/` 对应模块目录下
- 源文件放在 `code/src/` 对应目录下
- 测试项目模仿用户项目结构，便于验证功能
- 资源打包后生成的文件位于 `assets/textures/atlas/`、`config/`、`inc/` 中

## 注意事项

1. 构建前请确保已正确配置 vcpkg 和 CMake
2. `build/`、`out/`、`doxygen_output/` 为生成目录，不应提交到版本控制
3. 旧目录中的代码可能已过时，请以 `code/` 中的实现为准

---

*本文档基于项目当前结构编写，随着项目发展可能会有所变化。*