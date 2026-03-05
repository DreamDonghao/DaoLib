# Dao CLI 工具文档

Dao CLI 是一个命令行工具，用于辅助开发 Dao 框架项目。它提供了项目创建、页面生成、纹理打包和库安装等功能。

## 📋 重要提示

**命令执行位置说明**

| 命令 | 执行位置 | 说明 |
|------|---------|------|
| `dao create project` | 空目录 | 用于初始化新项目 |
| `dao create page` | 项目根目录 | 创建新页面 |
| `dao texture pack` | 项目根目录 | 打包纹理图集 |
| `dao install` | Dao 库源码目录 | 安装 dao 库 |

---

## 目录

- [安装](#安装)
- [命令列表](#命令列表)
  - [dao create - 创建项目或页面](#dao-create---创建项目或页面)
    - [dao create project - 创建项目](#dao-create-project---创建项目)
    - [dao create page - 创建页面](#dao-create-page---创建页面)
  - [dao texture pack - 打包纹理](#dao-texture-pack---打包纹理)
  - [dao install - 安装库](#dao-install---安装库)
  - [dao gif2png - 转换 GIF](#dao-gif2png---转换-gif) *(计划中)*

## 安装

使用 Go 构建工具：

```bash
go build -o dao.exe
```

将生成的 `dao.exe` 放置在系统 PATH 中。

---

## 命令列表

### `dao create` - 创建项目或页面

创建新的 Dao 项目或页面。

#### 子命令

- `project` - 创建新项目
- `page` - 创建新页面

---

#### `dao create project` - 创建项目

从模板创建新的 Dao 项目。

##### 📍 执行位置

**必须在空目录或仅包含配置文件的目录下执行**

```
empty_dir/          # ← 在此目录执行 dao create project
├── .git/           # ✅ 允许存在（可选）
├── .gitignore      # ✅ 允许存在（可选）
└── .vscode/        # ✅ 允许存在（可选）
```

**不允许的文件**：任何非配置文件将导致命令失败

##### 用法

```bash
dao create project [projectName]
```

##### 参数

- `projectName`: 项目名称

##### 示例

```bash
# 创建新项目
dao create project MyGame

# 在已初始化的 Git 仓库中创建项目
git init
dao create project MyApp
```

##### 工作流程

1. 检查当前目录是否为空（允许常见的配置文件如 `.git`、`.vscode` 等）
2. 如果目录非空且包含不允许的文件，命令将失败并提示
3. 从 embed 模板复制文件到当前目录
4. 替换占位符：
   - `{{.ProjectName}}` → 项目名称
   - `{{.InstallPath}}` → 安装路径
5. 生成项目结构

##### 生成的项目结构

```
project_root/          # 当前目录
├── CMakeLists.txt
├── main.cpp
├── vcpkg.json
├── assets/
│   └── textures/
│       └── input_images/
└── pages/
```

##### 错误示例

```bash
# ❌ 失败：目录包含非配置文件
echo "test" > test.txt
dao create project MyGame
# 错误: 当前目录不是空的，发现以下文件/目录: [test.txt]。

# ❌ 失败：目录已存在项目文件
dao create project Project1
dao create project Project2
# 错误: 当前目录不是空的，发现以下文件/目录: [CMakeLists.txt, main.cpp, ...]。
```

---

#### `dao create page` - 创建页面

生成新的页面类文件（.hpp 和 .cpp）。

##### 📍 执行位置

**必须在项目根目录下执行**

```
project_root/          # ← 在此目录执行 dao create page
├── CMakeLists.txt
├── main.cpp
├── assets/
└── pages/            # ✅ 目录已存在
```

##### 用法

```bash
dao create page [name]
```

##### 参数

- `name`: 页面名称（可包含命名空间）

##### 示例

```bash
# 创建 dao 命名空间下的页面
dao create page MyPage

# 创建自定义命名空间下的页面
dao create page App::HomePage
dao create page Game::GamePage
```

##### 工作流程

1. 解析命名空间和页面名
2. 将页面名转换为 snake_case 作为文件名
3. 在 `pages/` 目录下创建页面文件夹
4. 生成 .hpp 和 .cpp 文件，包含：
   - 命名空间声明
   - 继承自 `dao::GeneralPage`
   - 必需的虚函数实现

##### 生成的文件结构

```
pages/
└── my_page/
    ├── my_page.hpp
    └── my_page.cpp
```

---

### `dao texture pack` - 打包纹理

将多个 PNG 图片打包成纹理图集，并生成相关的 C++ 头文件。

##### 📍 执行位置

**必须在项目根目录下执行**

```
project_root/          # ← 在此目录执行 dao texture pack
├── CMakeLists.txt
├── assets/
│   └── textures/     # ✅ 必需目录
│       └── input_images/  # ✅ 必需目录（放置待打包的 PNG）
└── pages/
```

#### 用法

```bash
dao texture pack
```

#### 工作流程

1. **扫描输入图片**
   - 从 `assets/textures/input_images/` 目录读取所有 PNG 文件
   - 区分普通图片和序列帧（以 `sf` 开头的文件夹）

2. **生成图集**
   - 将图片按高度降序排序
   - 按最优方式排列到 2048x2048 的图集中
   - 保存图集图片到 `assets/textures/atlas/`

3. **生成元数据**
   - 创建 `assets/textures/config/atlas_meta.json`
   - 包含每个图集帧的位置信息

4. **生成 C++ 头文件**
   - `assets/textures/inc/textures.inc` - 纹理 ID 枚举
   - `assets/textures/inc/texture_dates.inc` - 纹理数据数组

#### 目录结构

```
assets/
└── textures/
    ├── input_images/       # 输入的 PNG 图片
    ├── config/
    │   └── atlas_meta.json # 图集元数据
    ├── atlas/              # 生成的图集图片
    │   ├── atlas_0002.png
    │   └── ...
    └── inc/                # 生成的 C++ 头文件
        ├── textures.inc
        └── texture_dates.inc
```

#### 纹理命名规则

- 文件路径中的 `/` 和 `.` 转换为 `_`
- 转换为小写
- 例如：`ui/button_normal.png` → `ui_button_normal`

#### 序列帧支持

- 将序列帧放在以 `sf` 开头的文件夹中
- 例如：`sf_explosion/frame_001.png`
- 生成对应的开始和结束 ID：`sf_explosion_begin`, `sf_explosion_end`

---

### `dao install` - 安装库

安装 dao 库到指定路径。

##### 📍 执行位置

**必须在 Dao 库源码目录下执行**

```
Dao/                   # ← 在此目录执行 dao install
├── cli_tool/          # cli 工具目录
├── code/              # dao 库源码
├── docs/              # 文档
└── test/              # 测试代码
```

#### 用法

```bash
dao install [buildType] [vcpkgCmakePath]
```

#### 参数

- `buildType`: 构建类型
- `vcpkgCmakePath`: vcpkg 工具链文件路径

#### 构建类型

- `All` - 构建所有类型
- `Debug` - 调试版本
- `Release` - 释放版本
- `MinSizeRel` - 最小体积版本
- `RelWithDebInfo` - 带调试信息的发布版本

#### 示例

```bash
# 安装 Release 版本
dao install Release C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# 构建所有类型
dao install All C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# 安装 Debug 版本
dao install Debug C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

#### 工作流程

1. 清理旧的构建目录
2. 运行 CMake 配置
3. 执行构建
4. 安装到指定路径
5. 生成安装路径文件供其他命令使用

---

### `dao gif2png` - 转换 GIF

将 GIF 动画转换为 PNG 序列帧。*(计划中，代码已实现但未注册到命令树)*

---

## 完整命令树

```
dao
├── create
│   ├── project [name]        # 📍 空目录 - 创建项目
│   └── page [name]           # 📍 项目根目录 - 创建页面
├── texture
│   └── pack                  # 📍 项目根目录 - 打包纹理
└── install [buildType] [path]# 📍 Dao 源码目录 - 安装库
```

## 验证当前目录

执行命令前，可以使用以下命令验证当前目录：

```bash
# Windows
dir

# Linux/Mac
ls -la
```

**项目根目录应该包含**：
- `CMakeLists.txt`
- `main.cpp`
- `assets/` 目录
- `pages/` 目录

**项目创建目录应该为空**（或仅包含配置文件）：
- `.git/` (可选)
- `.gitignore` (可选)
- `.vscode/` (可选)

**Dao 源码目录应该包含**：
- `cli_tool/` 目录
- `code/` 目录
- `docs/` 目录
- `test/` 目录

## 依赖

- Go 1.25+
- [github.com/spf13/cobra](https://github.com/spf13/cobra) - 命令行框架

## 项目结构

```
cli_tool/
├── main.go              # 主入口和命令定义
├── cmdRun/             # 命令实现
│   ├── install.go      # 安装命令
│   ├── create_project.go  # 创建项目
│   ├── create_page.go     # 创建页面
│   ├── packatlas.go    # 打包图集
│   ├── atlas2inc.go    # 生成头文件
│   └── gif2png.go      # GIF 转 PNG
├── atlas/              # 纹理图集相关
│   ├── pack.go         # 打包逻辑
│   ├── incgen.go       # 头文件生成
│   └── meta.go         # 元数据结构
├── util/               # 工具函数
│   └── enum.go         # 枚举名称转换
└── templates/          # 项目模板
    └── project/
```

## 许可证

(请根据实际情况添加许可证信息)