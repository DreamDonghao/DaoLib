package cmdRun

import (
	"dao/atlas"
	"fmt"
)

// RunAtlas2Inc 生成图集相关的 C++ 头文件
func RunAtlas2Inc() {
	inputJSON := "assets/textures/config/atlas_meta.json"
	meta, err := atlas.LoadMeta(inputJSON)
	if err != nil {
		fmt.Println("加载图集元数据失败:", err)
		return
	}

	if err := atlas.GenerateInc(meta); err != nil {
		fmt.Println("生成头文件失败:", err)
		return
	}

	fmt.Println("inc 生成完成")
}
