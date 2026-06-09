// Package cmd 提供 CLI 命令定义。
package cmd

import (
	"fmt"

	"dao/pkg/create"
	"dao/pkg/install"
	"dao/pkg/texture"

	"github.com/spf13/cobra"
)

// buildTypes 有效的构建类型列表
var buildTypes = map[string]bool{
	"All": true, "Debug": true, "Release": true,
	"MinSizeRel": true, "RelWithDebInfo": true,
}

// Execute 执行根命令
func Execute() error {
	return rootCmd.Execute()
}

// rootCmd 根命令
var rootCmd = &cobra.Command{
	Use:   "dao",
	Short: "Dao CLI 工具",
}

// installCmd 安装命令
var installCmd = &cobra.Command{
	Use:   "install [buildType] [vcpkgCmakePath]",
	Short: "安装 dao 库",
	Long: `安装 dao 库到指定路径。

可用的构建类型：
  All            - 构建所有类型
  Debug          - 调试版本
  Release        - 发布版本
  MinSizeRel     - 最小体积版本
  RelWithDebInfo - 带调试信息的发布版本`,
	Args: cobra.ExactArgs(2),
	RunE: func(cmd *cobra.Command, args []string) error {
		buildType := args[0]
		vcpkgCmakePath := args[1]

		if !buildTypes[buildType] {
			return fmt.Errorf("无效构建类型: %s (可选: All, Debug, Release, MinSizeRel, RelWithDebInfo)", buildType)
		}

		if buildType == "All" {
			for typeName := range buildTypes {
				if typeName != "All" {
					install.Run(typeName, vcpkgCmakePath)
				}
			}
		} else {
			install.Run(buildType, vcpkgCmakePath)
		}
		return nil
	},
}

// createCmd 创建命令
var createCmd = &cobra.Command{
	Use:   "create",
	Short: "创建内容",
}

// createProjectCmd 创建项目命令
var createProjectCmd = &cobra.Command{
	Use:   "project [name]",
	Short: "在当前目录创建新项目",
	Long: `在当前目录创建新项目。

此命令会将项目文件直接放置在当前目录中。
当前目录必须为空（除了常见的隐藏文件如 .git、.vscode 等）。

示例：
  在空目录中创建项目:
    $ dao create project MyGame`,
	Args: cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		projectName := args[0]
		err := create.Project(projectName)
		if err != nil {
			fmt.Println("创建项目失败:", err)
			return
		}
		fmt.Println("项目创建成功:", projectName)
	},
}

// createPageCmd 创建页面命令
var createPageCmd = &cobra.Command{
	Use:   "page [name]",
	Short: "创建新页面",
	Long: `创建新页面。

示例：
  dao create page MyPage          - 创建 dao::MyPage
  dao create page App::HomePage   - 创建 App::HomePage`,
	Args: cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		pageName := args[0]
		create.Page(pageName)
	},
}

// createTrayCmd 创建托盘命令
var createTrayCmd = &cobra.Command{
	Use:   "tray",
	Short: "创建托盘",
	Long: `创建托盘。

示例：
  dao create tray`,
	Args: cobra.ExactArgs(0),
	Run: func(cmd *cobra.Command, args []string) {
		if err := create.Tray(); err != nil {
			fmt.Println("创建托盘失败:", err)
		}
	},
}

// textureCmd 纹理命令
var textureCmd = &cobra.Command{
	Use:   "texture",
	Short: "纹理相关命令",
}

// packCmd 打包纹理命令
var packCmd = &cobra.Command{
	Use:   "pack",
	Short: "打包纹理",
	Args:  cobra.NoArgs,
	Run: func(cmd *cobra.Command, args []string) {
		texture.Pack()
	},
}

func init() {
	rootCmd.AddCommand(installCmd)

	rootCmd.AddCommand(createCmd)
	createCmd.AddCommand(createProjectCmd)
	createCmd.AddCommand(createPageCmd)
	createCmd.AddCommand(createTrayCmd)

	rootCmd.AddCommand(textureCmd)
	textureCmd.AddCommand(packCmd)
}
