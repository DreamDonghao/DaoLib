// Package texture 提供纹理打包功能。
package texture

import (
	"fmt"

	"dao/pkg/texture/atlas"
)

// Pack 打包纹理图集并生成头文件。
func Pack() {
	inputDir := "assets/textures/input_images"
	outputJSON := "assets/textures/config/atlas_meta.json"
	outputBase := "assets/textures/atlas"

	err := atlas.Pack(inputDir, outputJSON, outputBase)
	if err != nil {
		fmt.Println("打包纹理失败:", err)
		return
	}
	fmt.Println("图集生成完成")

	meta, err := atlas.LoadMeta(outputJSON)
	if err != nil {
		fmt.Println("加载图集元数据失败:", err)
		return
	}

	if err := atlas.GenerateInc(meta); err != nil {
		fmt.Println("生成头文件失败:", err)
		return
	}
	fmt.Println("头文件生成完成")
}