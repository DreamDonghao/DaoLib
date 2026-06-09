// Dao CLI 工具 - 用于辅助开发 Dao 框架项目的命令行工具。
package main

import (
	"os"

	"dao/cmd"
)

func main() {
	if err := cmd.Execute(); err != nil {
		os.Exit(1)
	}
}