package cmdRun

import (
	"dao/atlas"
	"fmt"
)

func RunAtlas2Inc() {
	inputJSON := "assets/textures/config/atlas_meta.json"
	meta, err := atlas.LoadMeta(inputJSON)
	if err != nil {
		panic(err)
	}

	if err := atlas.GenerateInc(meta); err != nil {
		panic(err)
	}

	fmt.Println("inc 生成完成")
}
