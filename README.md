# DaoUI

[中文](README.zh.md) | [English](README.md)

A cross-platform C++ GUI development framework for building desktop applications, utility software, 2D games, and other visual programs.

---

📘 Documentation and Tutorials:  
https://dreamdonghao.github.io/DaoUI/en/html

Through this documentation, you can learn how to use DaoUI, understand its design philosophy, and participate in project development.

---

## Project Introduction

DaoUI is a cross-platform GUI library built with C++, aiming to provide a lightweight, high-performance application development solution with a low learning curve.

The framework is built on SDL3, supports hardware acceleration, and all rendering is accomplished through vertex batching. When used appropriately, it delivers excellent performance, making it suitable not only for desktop application development but also for 2D games or other real-time visualization projects.

DaoUI is not positioned as a full-fledged game engine, nor does it provide a vast array of pre-built controls like Qt. Currently, it offers the following core capabilities:

- Texture management
- Geometric shape rendering
- Basic interactive components
- Page and window structure support

The design philosophy is: provide only the essential underlying capabilities, allowing developers greater freedom to build their own UI or systems.

With basic C++ experience, you can get started in a relatively short time. Commonly used components are under continuous development.

Whether you're a beginner or an experienced developer, you are welcome to participate in the development and improvement of DaoUI for practice, learning, and building projects.

---

## Usage Instructions

Before you begin, ensure that **CMake** and **vcpkg** are installed on your system and accessible from the command line.

### 1. Download and Preparation

Download the compressed package for your platform from the project's Releases page and extract it to a **stable, infrequently moved** directory.

The **`dao`** executable in the package is the project's command-line tool. All subsequent project builds and resource processing will depend on it.
It is recommended to add this directory to your system's PATH environment variable for convenient use from any location.

------

### 2. Installing dao

Execute:

```bash
dao install [buildType] [vcpkgCmakePath]
```

Parameter explanation:

- `[buildType]`: Build type
  Options:
    - `All`
    - `Debug`
    - `Release`
    - `MinSizeRel`
    - `RelWithDebInfo`
- `[vcpkgCmakePath]`: Path to `vcpkg.cmake`, for example:

```bash
dao install All ~/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Successful completion of the command indicates a successful installation.

------

### 3. Creating a Project

Create a new project using the following command:

```bash
dao create project [projectName]
```

Example:

```bash
dao create project hello_dao
```

The generated project structure is as follows:

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

### 4. Packaging Image Resources

If you need to use image resources:

1. Place the images into the `assets/textures/input_images` directory.
2. Execute the following command in the project's root directory:

```bash
dao texture pack
```

This command will automatically complete the atlas packaging and generate the related files.

------

### 5. More Documentation

For complete project documentation and advanced usage, please visit:

https://dreamdonghao.github.io/DaoUI/en/html