package cmdRun

import (
	"dao/atlas"
	"fmt"
)

func RunPackAtlas() {

	inputDir := "assets/textures/input_images"
	outputJSON := "assets/textures/config/atlas_meta.json"
	outputBase := "assets/textures/atlas"

	err := atlas.PackAtlas(inputDir, outputJSON, outputBase)
	if err != nil {
		panic(err)
	}

	fmt.Println("图集生成完成")
}
