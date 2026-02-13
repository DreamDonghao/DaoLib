package main

import (
	"dao/cmdRun"
	"os"

	"github.com/spf13/cobra"
)

var rootCmd = &cobra.Command{
	Use: "dao",
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
		cmdRun.CreateProject(projectName)
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
	rootCmd.AddCommand(createCmd)
	createCmd.AddCommand(createProjectCmd)

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
