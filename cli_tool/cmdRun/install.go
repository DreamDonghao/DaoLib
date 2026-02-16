package cmdRun

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

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

func Install(buildType string, vcpkgCmakePath string) {
	exePath, _ := os.Executable()
	exeDir := filepath.Dir(exePath)
	sourceDir := filepath.Join(exeDir, "code")

	buildDir := filepath.Join(exeDir, "build", strings.ToLower(buildType))
	installDir := filepath.Join(exeDir, "install")

	err := os.WriteFile(filepath.Join(exeDir, "install_path"),
		[]byte(strings.ReplaceAll(installDir, "\\", "/")), 0644)
	if err != nil {
		log.Fatal(err)
	}

	err = os.RemoveAll(buildDir)
	if err != nil {

	}

	fmt.Println("cmake", "-S", sourceDir, "-B", buildDir, "-DCMAKE_TOOLCHAIN_FILE="+vcpkgCmakePath)

	run("cmake", "-S", sourceDir, "-B", buildDir,
		"-DCMAKE_TOOLCHAIN_FILE="+vcpkgCmakePath,
	)

	run("cmake", "--build", buildDir, "--config", buildType)
	run("cmake", "--install", buildDir, "--prefix", installDir, "--config", buildType)

	fmt.Println("构建完成！安装路径：", installDir)
}
