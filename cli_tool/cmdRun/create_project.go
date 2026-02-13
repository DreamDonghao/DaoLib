package cmdRun

import (
	"bytes"
	"embed"
	"io/fs"
	"log"
	"os"
	"path/filepath"
	"strings"
)

//go:embed templates/project/*
var templateFS embed.FS

func CreateProject(projectName string) error {
	targetRoot := filepath.Join(".", projectName)

	// 创建项目根目录
	if err := os.MkdirAll(targetRoot, 0755); err != nil {
		return err
	}

	// 遍历 embed 中的 project 目录
	return fs.WalkDir(templateFS, "templates/project", func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		// 计算相对路径
		relPath, err := filepath.Rel("templates/project", path)
		if err != nil {
			return err
		}

		// 输出路径
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
			log.Fatal(err)
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
