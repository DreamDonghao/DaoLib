// Package create 提供项目、页面、托盘的创建功能。
package create

import (
	"bytes"
	"embed"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
)

//go:embed template/*
var templateFS embed.FS

// allowedFiles 表示创建项目时允许存在于目录中的文件或目录。
var allowedFiles = map[string]bool{
	".git":          true,
	".gitignore":    true,
	".vscode":       true,
	".idea":         true,
	".vs":           true,
	".DS_Store":     true,
	"Thumbs.db":     true,
	".editorconfig": true,
	".clang-format": true,
}

// Project 在当前目录创建新项目。
func Project(projectName string) error {
	currentDir := "."

	entries, err := os.ReadDir(currentDir)
	if err != nil {
		return fmt.Errorf("检查目录失败: %v", err)
	}

	var unsafeEntries []string
	for _, entry := range entries {
		if !allowedFiles[entry.Name()] {
			unsafeEntries = append(unsafeEntries, entry.Name())
		}
	}

	if len(unsafeEntries) > 0 {
		return fmt.Errorf("当前目录不是空的，发现以下文件/目录: %v。\n请在空目录中运行此命令", unsafeEntries)
	}

	return fs.WalkDir(templateFS, "template/project", func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		if path == "template/project" {
			return nil
		}

		relPath, err := filepath.Rel("template/project", path)
		if err != nil {
			return err
		}

		outPath := filepath.Join(currentDir, relPath)

		if d.IsDir() {
			return os.MkdirAll(outPath, 0755)
		}

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

		if isTextFile(path, data) {
			content := string(data)
			content = strings.ReplaceAll(content, "{{.ProjectName}}", projectName)
			content = strings.ReplaceAll(content, "{{.InstallPath}}", string(installPath))
			data = []byte(content)
		}

		return os.WriteFile(outPath, data, 0644)
	})
}

// isTextFile 判断是否为文本文件。
func isTextFile(path string, data []byte) bool {
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

	if ext == "" {
		return !bytes.Contains(data, []byte{0})
	}

	return false
}
