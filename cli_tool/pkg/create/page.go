package create

import (
	"fmt"
	"os"
	"strings"
)

// Page 创建新页面。
func Page(name string) {
	idx := strings.LastIndex(name, "::")

	var namespace string
	var pageName string
	if idx == -1 {
		pageName = name
	} else {
		namespace = name[:idx]
		pageName = name[idx+2:]
	}

	// 文件名和文件夹名直接使用页面类名
	dir := "./pages/" + pageName

	if err := os.MkdirAll(dir, 0755); err != nil {
		fmt.Println("创建目录失败:", err)
		return
	}

	includePath := pageName + "/" + pageName + ".hpp"

	replacer := strings.NewReplacer(
		"{{.Namespace}}", namespace,
		"{{.PageName}}", pageName,
		"{{.IncludePath}}", includePath,
	)

	hppData, err := templateFS.ReadFile("template/page/page.hpp")
	if err != nil {
		fmt.Println("读取模板失败:", err)
		return
	}

	cppData, err := templateFS.ReadFile("template/page/page.cpp")
	if err != nil {
		fmt.Println("读取模板失败:", err)
		return
	}

	hppPath := dir + "/" + pageName + ".hpp"
	if err := os.WriteFile(hppPath, []byte(replacer.Replace(string(hppData))), 0644); err != nil {
		fmt.Println("写入 hpp 文件失败:", err)
		return
	}

	cppPath := dir + "/" + pageName + ".cpp"
	if err := os.WriteFile(cppPath, []byte(replacer.Replace(string(cppData))), 0644); err != nil {
		fmt.Println("写入 cpp 文件失败:", err)
		return
	}

	fmt.Println("页面创建成功:", dir)
}