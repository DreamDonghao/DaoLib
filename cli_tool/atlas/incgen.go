package atlas

import (
	"fmt"
	"os"
	"sort"
	"strings"

	"dao/util"
)

func GenerateInc(data *AtlasMeta) error {

	textureID := 2
	texturesInc := "assets/textures/inc/textures.inc"
	textureDataInc := "assets/textures/inc/texture_dates.inc"

	tf, err := os.Create(texturesInc)
	if err != nil {
		return err
	}
	defer tf.Close()

	tf.WriteString("#pragma once\nusing TextureID = unsigned int;\n")

	// images 排序
	imageKeys := make([]string, 0, len(data.Images))
	for k := range data.Images {
		imageKeys = append(imageKeys, k)
	}
	sort.Strings(imageKeys)

	for _, key := range imageKeys {
		cppName := util.ToEnumName(key)
		tf.WriteString(fmt.Sprintf("constexpr TextureID %s = %d;\n", cppName, textureID))
		textureID++
	}

	// sequences 排序
	seqKeys := make([]string, 0, len(data.SequenceFrames))
	for k := range data.SequenceFrames {
		seqKeys = append(seqKeys, k)
	}
	sort.Strings(seqKeys)

	for _, seqName := range seqKeys {
		frames := data.SequenceFrames[seqName]

		cppName := util.ToEnumName(seqName)
		beginID := textureID
		endID := textureID + len(frames)

		tf.WriteString(fmt.Sprintf("constexpr TextureID %s_begin = %d;\n", cppName, beginID))
		tf.WriteString(fmt.Sprintf("constexpr TextureID %s_end = %d;\n", cppName, endID))

		textureID += len(frames)
	}

	// texture_datas.inc
	var regionEntries []string

	for _, key := range imageKeys {
		v := data.Images[key]
		entry := fmt.Sprintf(
			`{"%s",%d,%.8f,%.8f,%.8f,%.8f}`,
			"./assets/textures/atlas/"+v.AtlasFile,
			v.AtlasID,
			v.Left/2048,
			v.Top/2048,
			v.Right/2048,
			v.Bottom/2048,
		)
		regionEntries = append(regionEntries, entry)
	}

	for _, seqName := range seqKeys {
		frames := data.SequenceFrames[seqName]
		for _, v := range frames {
			entry := fmt.Sprintf(
				`{"%s",%d,%.8f,%.8f,%.8f,%.8f}`,
				v.AtlasFile,
				v.AtlasID,
				v.Left/2048,
				v.Top/2048,
				v.Right/2048,
				v.Bottom/2048,
			)
			regionEntries = append(regionEntries, entry)
		}
	}

	df, err := os.Create(textureDataInc)
	if err != nil {
		return err
	}
	defer df.Close()

	df.WriteString(strings.Join(regionEntries, ","))

	return nil
}
