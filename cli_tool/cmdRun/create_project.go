package cmdRun

import (
	"bytes"
	"embed"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
)

//go:embed templates/project/*
var templateFS embed.FS

// isSafeForProjectCreation 检查目录是否可以安全地创建项目
// 允许存在常见的配置文件和隐藏目录
func isSafeForProjectCreation(dir string) (bool, []string, error) {
	entries, err := os.ReadDir(dir)
	if err != nil {
		return false, nil, err
	}

	// 允许的文件/目录列表
	allowed := map[string]bool{
		".git":          true,
		".gitignore":    true,
		".vscode":       true,
		".idea":         true,
		".vs":           true,
		".DS_Store":     true,
		"Thumbs.db":     true,
		".editorconfig": true,
		".clang-format": true,
		// 可以根据需要添加更多
	}

	var unsafeEntries []string
	for _, entry := range entries {
		if !allowed[entry.Name()] {
			unsafeEntries = append(unsafeEntries, entry.Name())
		}
	}

	return len(unsafeEntries) == 0, unsafeEntries, nil
}

// CreateProject 创建新项目（直接在当前目录）
func CreateProject(projectName string) error {
	currentDir := "."

	// 检查目录是否安全
	isSafe, unsafeEntries, err := isSafeForProjectCreation(currentDir)
	if err != nil {
		return fmt.Errorf("检查目录失败: %v", err)
	}
	if !isSafe {
		return fmt.Errorf(
			"当前目录不是空的，发现以下文件/目录: %v。\n请在空目录中运行此命令",
			unsafeEntries,
		)
	}

	// 直接在当前目录放置文件（不再创建子目录）
	targetRoot := currentDir

	// 遍历 embed 中的 project 目录
	return fs.WalkDir(templateFS, "templates/project", func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		// 跳过模板根目录本身
		if path == "templates/project" {
			return nil
		}

		// 计算相对路径
		relPath, err := filepath.Rel("templates/project", path)
		if err != nil {
			return err
		}

		// 输出路径直接在当前目录
		outPath := filepath.Join(targetRoot, relPath)

		// 如果是目录 → 创建
		if d.IsDir() {
			return os.MkdirAll(outPath, 0755)
		}

		// 如果是文件 → 读取 embed 内容
		data, err := templateFS.ReadFile(path)
		if err != nil {
			return err
		}

		exePath, _ := os.Executable()
		exeDir := filepath.Dir(exePath)

		installPath, err := os.ReadFile(filepath.Join(exeDir, "install_path"))
		if err != nil {
			return fmt.Errorf("读取安装路径失败: %v", err)
		}

		// 如果是文本文件，替换占位符
		if isTextFile(path, data) {
			content := string(data)
			content = strings.ReplaceAll(content, "{{.ProjectName}}", projectName)
			content = strings.ReplaceAll(content, "{{.InstallPath}}", string(installPath))
			data = []byte(content)
		}

		// 写入到本地
		return os.WriteFile(outPath, data, 0644)
	})
}

// isTextFile 判断是否为文本文件
func isTextFile(path string, data []byte) bool {
	// 根据扩展名判断
	ext := strings.ToLower(filepath.Ext(path))
	textExts := map[string]bool{
		".go": true, ".mod": true, ".sum": true, ".txt": true,
		".md": true, ".json": true, ".yaml": true, ".yml": true,
		".toml": true, ".xml": true, ".html": true, ".css": true,
		".js": true, ".ts": true, ".sh": true, ".env": true,
		".gitignore": true, ".dockerignore": true, ".c": true, ".cpp": true,
		".h": true, ".hpp": true,
	}

	if textExts[ext] {
		return true
	}

	// 无扩展名文件，检查是否包含 null 字节
	if ext == "" {
		return !bytes.Contains(data, []byte{0})
	}

	return false
}
