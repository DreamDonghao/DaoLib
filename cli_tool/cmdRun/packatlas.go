package cmdRun

import (
	"dao/atlas"
	"fmt"
)

// RunPackAtlas 打包纹理图集
func RunPackAtlas() {
	inputDir := "assets/textures/input_images"
	outputJSON := "assets/textures/config/atlas_meta.json"
	outputBase := "assets/textures/atlas"

	err := atlas.PackAtlas(inputDir, outputJSON, outputBase)
	if err != nil {
		fmt.Println("打包纹理失败:", err)
		return
	}

	fmt.Println("图集生成完成")
}
