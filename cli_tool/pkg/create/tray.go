package create

import (
	"fmt"
	"os"
)

// Tray 创建托盘类文件。
func Tray() error {
	if err := os.MkdirAll("./tray", 0755); err != nil {
		fmt.Println("创建目录失败:", err)
		return err
	}

	hppData, err := templateFS.ReadFile("template/tray/tray.hpp")
	if err != nil {
		fmt.Println("读取模板失败:", err)
		return err
	}

	cppData, err := templateFS.ReadFile("template/tray/tray.cpp")
	if err != nil {
		fmt.Println("读取模板失败:", err)
		return err
	}

	hppPath := "./tray/AppTray.hpp"
	cppPath := "./tray/AppTray.cpp"

	if err := os.WriteFile(hppPath, hppData, 0644); err != nil {
		fmt.Println("写入 hpp 文件失败:", err)
		return err
	}
	if err := os.WriteFile(cppPath, cppData, 0644); err != nil {
		fmt.Println("写入 cpp 文件失败:", err)
		return err
	}

	fmt.Println("托盘创建成功: ./tray/")
	return nil
}