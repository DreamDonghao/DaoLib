# DaoLib

[中文](docs/zh/README.zh.md) | [English](docs/en/README.en.md)

A cross-platform C++ GUI development framework for building desktop applications, utility software, 2D games, and other visual programs.

一个跨平台的 C++ GUI 通用开发框架，可用于开发桌面应用、工具软件以及 2D 游戏等可视化程序。

---

📘 Documentation and Tutorials / 文档与教程：
https://dreamdonghao.github.io/DaoLib/en/html

Through this documentation, you can learn how to use DaoLib, understand its design philosophy, and participate in project development.

你可以通过该文档了解 DaoLib 的使用方式、设计理念，并参与项目开发。

---

## Project Introduction / 项目介绍

DaoLib is a cross-platform GUI library built with C++, aiming to provide a lightweight, high-performance application development solution with a low learning curve.

DaoLib 是基于 C++ 构建的跨平台 GUI 库，目标是提供一个轻量、高性能、低学习成本的应用开发方案。

The framework is built on SDL3, supports hardware acceleration, and all rendering is accomplished through vertex batching. When used appropriately, it delivers excellent performance, making it suitable not only for desktop application development but also for 2D games or other real-time visualization projects.

框架基于 SDL3 构建，支持硬件加速，所有渲染均采用顶点批处理方式完成。在合理使用的情况下具备良好的性能表现，因此不仅适用于桌面应用开发，也可用于 2D 游戏或其他实时可视化项目。

DaoLib is not positioned as a full-fledged game engine, nor does it provide a vast array of pre-built controls like Qt. Currently, it offers the following core capabilities:

DaoLib 并不定位为完整的游戏引擎，也不像 Qt 提供大量现成控件。当前主要提供以下基础能力：

- Texture management / 纹理管理
- Geometric shape rendering / 几何图形绘制
- Basic interactive components / 基础交互组件
- Page and window structure support / 页面与窗口结构支持

The design philosophy is: provide only the essential underlying capabilities, allowing developers greater freedom to build their own UI or systems.

设计理念是：只提供必要的底层能力，让开发者可以更自由地构建自己的 UI 或系统。

With basic C++ experience, you can get started in a relatively short time. Commonly used components are under continuous development.

只要具备基础 C++ 使用经验，即可在较短时间内上手。常用组件仍在持续开发中。

Whether you're a beginner or an experienced developer, you are welcome to participate in the development and improvement of DaoLib for practice, learning, and building projects.

无论是初学者还是有经验的开发者，都欢迎参与 DaoLib 的开发与改进，用于实践、学习和构建项目。

---

## Usage Instructions / 使用说明

Before you begin, ensure that **CMake** and **vcpkg** are installed on your system and accessible from the command line.

在开始之前，请确保系统环境中已安装 **CMake** 与 **vcpkg**，并可以在命令行中直接调用。

### 1. Download and Preparation / 下载与准备

Download the compressed package for your platform from the project's Releases page and extract it to a **stable, infrequently moved** directory.

从项目的 Releases 页面下载对应平台的压缩包并解压到一个**稳定、不经常移动**的目录。

The **`dao`** executable in the package is the project's command-line tool. All subsequent project builds and resource processing will depend on it. It is recommended to add this directory to your system's PATH environment variable for convenient use from any location.

压缩包中的 **`dao`** 是项目的命令行工具，后续所有项目构建与资源处理都会依赖它。建议将该目录加入系统环境变量（PATH），以便在任意位置使用。

------

### 2. Installing dao / 安装 dao

Execute / 执行：

```bash
dao install [buildType] [vcpkgCmakePath]
```

Parameter explanation / 参数说明：

- `[buildType]`: Build type / 构建类型
  Options / 可选值：
    - `All`
    - `Debug`
    - `Release`
    - `MinSizeRel`
    - `RelWithDebInfo`
- `[vcpkgCmakePath]`: Path to `vcpkg.cmake` / `vcpkg.cmake` 的路径，例如：

```bash
dao install All ~/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Successful completion of the command indicates a successful installation.

命令正常执行完成即表示安装成功。

------

### 3. Creating a Project / 创建项目

Create a new project using the following command / 使用以下命令创建新项目：

```bash
dao create project [projectName]
```

Example / 示例：

```bash
dao create project hello_dao
```

The generated project structure is as follows / 生成的项目结构如下：

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

### 4. Packaging Image Resources / 图片资源打包

If you need to use image resources / 如果需要使用图片资源：

1. Place the images into the `assets/textures/input_images` directory. / 将图片放入 `assets/textures/input_images` 目录
2. Execute the following command in the project's root directory / 在项目根目录执行：

```bash
dao texture pack
```

This command will automatically complete the atlas packaging and generate the related files.

该命令会自动完成图集打包与相关文件生成。

------

### 5. More Documentation / 更多文档

For complete project documentation and advanced usage, please visit / 完整项目文档与进阶用法请访问：

https://dreamdonghao.github.io/DaoLib/en/html