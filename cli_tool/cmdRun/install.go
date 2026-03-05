package cmdRun

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

// run 执行命令
func run(name string, args ...string) {
	cmd := exec.Command(name, args...)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	if err := cmd.Run(); err != nil {
		fmt.Printf("命令 %s 执行失败: %v\n", name, err)
		os.Exit(1)
	}
}

// Install 安装 dao 库
func Install(buildType string, vcpkgCmakePath string) {
	exePath, _ := os.Executable()
	exeDir := filepath.Dir(exePath)
	sourceDir := filepath.Join(exeDir, "code")

	buildDir := filepath.Join(exeDir, "build", strings.ToLower(buildType))
	installDir := filepath.Join(exeDir, "install")

	err := os.WriteFile(filepath.Join(exeDir, "install_path"),
		[]byte(strings.ReplaceAll(installDir, "\\", "/")), 0644)
	if err != nil {
		fmt.Println("写入安装路径失败:", err)
		return
	}

	err = os.RemoveAll(buildDir)
	if err != nil {
		fmt.Println("清理构建目录失败:", err)
	}

	fmt.Println("cmake", "-S", sourceDir, "-B", buildDir, "-DCMAKE_TOOLCHAIN_FILE="+vcpkgCmakePath)

	run("cmake", "-S", sourceDir, "-B", buildDir,
		"-DCMAKE_TOOLCHAIN_FILE="+vcpkgCmakePath,
	)

	run("cmake", "--build", buildDir, "--config", buildType)
	run("cmake", "--install", buildDir, "--prefix", installDir, "--config", buildType)

	fmt.Println("构建完成！安装路径：", installDir)
}
