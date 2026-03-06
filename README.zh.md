# DaoLib

[中文](README.zh.md) | [English](README.md)

一个跨平台的 C++ GUI 通用开发框架，可用于开发桌面应用、工具软件以及 2D 游戏等可视化程序。

---

📘 文档与教程：  
https://dreamdonghao.github.io/DaoLib/zh/html

你可以通过该文档了解 DaoLib 的使用方式、设计理念，并参与项目开发。

---

## 项目介绍

DaoLib 是基于 C++ 构建的跨平台 GUI 库，目标是提供一个轻量、高性能、低学习成本的应用开发方案。

框架基于 SDL3 构建，支持硬件加速，所有渲染均采用顶点批处理方式完成。在合理使用的情况下具备良好的性能表现，因此不仅适用于桌面应用开发，也可用于 2D 游戏或其他实时可视化项目。

DaoLib 并不定位为完整的游戏引擎，也不像 Qt 提供大量现成控件。当前主要提供以下基础能力：

- 纹理管理
- 几何图形绘制
- 基础交互组件
- 页面与窗口结构支持

设计理念是：只提供必要的底层能力，让开发者可以更自由地构建自己的 UI 或系统。

只要具备基础 C++ 使用经验，即可在较短时间内上手。常用组件仍在持续开发中。

无论是初学者还是有经验的开发者，都欢迎参与 DaoLib 的开发与改进，用于实践、学习和构建项目。

---

## 使用说明

在开始之前，请确保系统环境中已安装 **CMake** 与 **vcpkg**，并可以在命令行中直接调用。

### 1. 下载与准备

从项目的 Releases 页面下载对应平台的压缩包并解压到一个**稳定、不经常移动**的目录。

压缩包中的 **`dao`** 是项目的命令行工具，后续所有项目构建与资源处理都会依赖它。
建议将该目录加入系统环境变量（PATH），以便在任意位置使用。

------

### 2. 安装 dao

执行：

```bash
dao install [buildType] [vcpkgCmakePath]
```

参数说明：

- `[buildType]`：构建类型
  可选值：
    - `All`
    - `Debug`
    - `Release`
    - `MinSizeRel`
    - `RelWithDebInfo`
- `[vcpkgCmakePath]`：`vcpkg.cmake` 的路径，例如：

```bash
dao install All ~/vcpkg/scripts/buildsystems/vcpkg.cmake
```

命令正常执行完成即表示安装成功。

------

### 3. 创建项目

使用以下命令创建新项目：

```bash
dao create project [projectName]
```

示例：

```bash
dao create project hello_dao
```

生成的项目结构如下：

```
hello_dao
│   CMakeLists.txt
│   main.cpp
│   vcpkg.json
│
├───assets
│   ├───textures
│   │   ├───atlas
│   │   ├───config
│   │   ├───inc
│   │   └───input_images
│   └───ttf
│
└───pages
        hello_dao_page.cpp
        hello_dao_page.hpp
```

------

### 4. 图片资源打包

如果需要使用图片资源：

1. 将图片放入 `assets/textures/input_images` 目录
2. 在项目根目录执行：

```bash
dao texture pack
```

该命令会自动完成图集打包与相关文件生成。

------

### 5. 更多文档

完整项目文档与进阶用法请访问：

https://dreamdonghao.github.io/DaoLib/zh/html
