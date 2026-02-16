package main

import (
	"dao/cmdRun"
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

var rootCmd = &cobra.Command{
	Use: "dao",
}

var installCmd = &cobra.Command{
	Use:   "install [buildType] [vcpkgCmakePath]",
	Short: "安装 dao",
	Args:  cobra.ExactArgs(2),
	RunE: func(cmd *cobra.Command, args []string) error {
		buildType := args[0]
		vcpkgCmakePath := args[1]
		valid := map[string]bool{
			"All": true, "Debug": true, "Release": true, "MinSizeRel": true, "RelWithDebInfo": true,
		}
		if !valid[buildType] {
			return fmt.Errorf("无效构建类型: %s (可选:All, Debug, Release, MinSizeRel, RelWithDebInfo)\n", buildType)
		}
		if buildType == "All" {
			for typeName := range valid {
				if typeName != "All" {
					cmdRun.Install(typeName, vcpkgCmakePath)
				}
			}
		} else {
			cmdRun.Install(buildType, vcpkgCmakePath)
		}
		return nil
	},
}

var createCmd = &cobra.Command{
	Use:   "create",
	Short: "创建",
	Run: func(cmd *cobra.Command, args []string) {

	},
}
var createProjectCmd = &cobra.Command{
	Use:   "project [name]",
	Short: "创建项目",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		projectName := args[0]
		err := cmdRun.CreateProject(projectName)
		if err != nil {
			return
		}
	},
}

var createPageCmd = &cobra.Command{
	Use:   "page [name]",
	Short: "创建页面",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		pageName := args[0]
		cmdRun.CreatePage(pageName)
	},
}

var textureCmd = &cobra.Command{
	Use:   "texture",
	Short: "纹理相关命令",
	Run: func(cmd *cobra.Command, args []string) {

	},
}

var packCmd = &cobra.Command{
	Use:   "pack",
	Short: "打包纹理",
	Run: func(cmd *cobra.Command, args []string) {
		cmdRun.RunPackAtlas()
		cmdRun.RunAtlas2Inc()
	},
}

func init() {
	rootCmd.AddCommand(installCmd)
	rootCmd.AddCommand(createCmd)
	createCmd.AddCommand(createProjectCmd)
	createCmd.AddCommand(createPageCmd)
	rootCmd.AddCommand(textureCmd)
	textureCmd.AddCommand(packCmd)

}

func Execute() {
	if err := rootCmd.Execute(); err != nil {
		os.Exit(1)
	}
}

func main() {
	Execute()

	//switch os.Args[1] {
	//case "split_gif":
	//	cmdRun.RunGif2Png(os.Args[2:])
	//default:
	//	fmt.Println("未知命令:", os.Args[1])
	//}

}
