// Package atlas 提供纹理图集打包和元数据处理功能。
package atlas

import (
	"encoding/json"
	"fmt"
	"image"
	"image/png"
	"os"
	"path/filepath"
	"sort"
	"strings"
)

// 图集配置常量
const (
	padding     = 2
	atlasWidth  = 2048
	atlasHeight = 2048
)

// FrameMeta 表示图集中单个帧的元数据。
type FrameMeta struct {
	AtlasFile string  `json:"atlas_file"`
	AtlasID   int     `json:"atlas_id"`
	Left      float64 `json:"left"`
	Top       float64 `json:"top"`
	Right     float64 `json:"right"`
	Bottom    float64 `json:"bottom"`
}

// AtlasMeta 表示图集的完整元数据。
type AtlasMeta struct {
	Images         map[string]FrameMeta   `json:"images"`
	SequenceFrames map[string][]FrameMeta `json:"sequence_frames"`
}

// packItem 表示待打包的图片项。
type packItem struct {
	Rel    string
	Key    string
	Img    image.Image
	W      int
	H      int
	Folder string
}

// Pack 将 PNG 图片打包成图集。
func Pack(rootDir, outputJSON, outputBase string) error {
	fmt.Println("扫描 PNG:", rootDir)

	var imagesToPack []packItem
	sequenceGroups := make(map[string][]packItem)

	err := filepath.Walk(rootDir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if info.IsDir() {
			return nil
		}
		if !strings.HasSuffix(strings.ToLower(path), ".png") {
			return nil
		}

		file, err := os.Open(path)
		if err != nil {
			return nil
		}
		img, err := png.Decode(file)
		file.Close()
		if err != nil {
			return nil
		}

		rel, _ := filepath.Rel(rootDir, path)
		rel = filepath.ToSlash(rel)
		key := strings.ReplaceAll(strings.TrimSuffix(rel, ".png"), "/", "_")

		folderRel := filepath.ToSlash(filepath.Dir(rel))
		var folderKey string
		if strings.HasPrefix(filepath.Base(filepath.Dir(path)), "sf") {
			folderKey = folderRel
		} else {
			folderKey = filepath.Base(filepath.Dir(path))
		}

		item := packItem{
			Rel:    rel,
			Key:    key,
			Img:    img,
			W:      img.Bounds().Dx(),
			H:      img.Bounds().Dy(),
			Folder: folderKey,
		}

		if strings.HasPrefix(folderKey, "sf") {
			sequenceGroups[folderKey] = append(sequenceGroups[folderKey], item)
		} else {
			imagesToPack = append(imagesToPack, item)
		}

		return nil
	})

	if err != nil {
		return err
	}

	if len(imagesToPack) == 0 && len(sequenceGroups) == 0 {
		return fmt.Errorf("没有找到 PNG 文件")
	}

	for folder, items := range sequenceGroups {
		sort.Slice(items, func(i, j int) bool {
			return items[i].Rel < items[j].Rel
		})
		sequenceGroups[folder] = items
	}

	sort.Slice(imagesToPack, func(i, j int) bool {
		return imagesToPack[i].H > imagesToPack[j].H
	})

	allItems := append([]packItem{}, imagesToPack...)
	for _, v := range sequenceGroups {
		allItems = append(allItems, v...)
	}

	atlasIndex := 2
	cur := 0

	imageMeta := make(map[string]interface{})
	sequenceMeta := make(map[string][]interface{})

	for name := range sequenceGroups {
		sequenceMeta[name] = []interface{}{}
	}

	for cur < len(allItems) {
		atlasFileName := fmt.Sprintf("atlas_%04d.png", atlasIndex)
		atlasName := filepath.Join(outputBase, atlasFileName)

		atlasImage := image.NewRGBA(image.Rect(0, 0, atlasWidth, atlasHeight))

		x, y, rowH := 0, 0, 0
		i := cur

		for i < len(allItems) {
			item := allItems[i]
			w, h := item.W, item.H

			newLine := x+w > atlasWidth
			ny := y
			if newLine {
				ny = y + rowH + padding
			}

			if ny+h > atlasHeight {
				break
			}

			if newLine {
				x = 0
				y = ny
				rowH = 0
			}

			drawRect(atlasImage, item.Img, x, y)

			frameMeta := map[string]interface{}{
				"atlas_file": filepath.Base(atlasName),
				"atlas_id":   atlasIndex,
				"left":       x,
				"top":        y,
				"right":      x + w,
				"bottom":     y + h,
			}

			if strings.HasPrefix(item.Folder, "sf") {
				sequenceMeta[item.Folder] = append(sequenceMeta[item.Folder], frameMeta)
			} else {
				imageMeta[item.Key] = frameMeta
			}

			x += w + padding
			if h > rowH {
				rowH = h
			}
			i++
		}

		out, err := os.Create(atlasName)
		if err != nil {
			return err
		}
		if err := png.Encode(out, atlasImage); err != nil {
			out.Close()
			return err
		}
		out.Close()

		atlasIndex++
		cur = i
	}

	final := map[string]interface{}{
		"images":          imageMeta,
		"sequence_frames": sequenceMeta,
	}

	raw, err := json.MarshalIndent(final, "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(outputJSON, raw, 0644)
}

// drawRect 将源图片绘制到目标图片的指定位置。
func drawRect(dst image.Image, src image.Image, x, y int) {
	b := src.Bounds()
	for i := 0; i < b.Dx(); i++ {
		for j := 0; j < b.Dy(); j++ {
			dst.(*image.RGBA).Set(x+i, y+j, src.At(b.Min.X+i, b.Min.Y+j))
		}
	}
}

// LoadMeta 从 JSON 文件加载图集元数据。
func LoadMeta(path string) (*AtlasMeta, error) {
	raw, err := os.ReadFile(path)
	if err != nil {
		return nil, err
	}

	var meta AtlasMeta
	if err := json.Unmarshal(raw, &meta); err != nil {
		return nil, err
	}
	return &meta, nil
}

// GenerateInc 从图集元数据生成 C++ 头文件。
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

	imageKeys := make([]string, 0, len(data.Images))
	for k := range data.Images {
		imageKeys = append(imageKeys, k)
	}
	sort.Strings(imageKeys)

	for _, key := range imageKeys {
		cppName := toEnumName(key)
		tf.WriteString(fmt.Sprintf("constexpr TextureID %s = %d;\n", cppName, textureID))
		textureID++
	}

	seqKeys := make([]string, 0, len(data.SequenceFrames))
	for k := range data.SequenceFrames {
		seqKeys = append(seqKeys, k)
	}
	sort.Strings(seqKeys)

	for _, seqName := range seqKeys {
		frames := data.SequenceFrames[seqName]

		cppName := toEnumName(seqName)
		beginID := textureID
		endID := textureID + len(frames)

		tf.WriteString(fmt.Sprintf("constexpr TextureID %s_begin = %d;\n", cppName, beginID))
		tf.WriteString(fmt.Sprintf("constexpr TextureID %s_end = %d;\n", cppName, endID))

		textureID += len(frames)
	}

	var regionEntries []string

	for _, key := range imageKeys {
		v := data.Images[key]
		entry := fmt.Sprintf(
			`{"%s",%d,%.8f,%.8f,%.8f,%.8f}`,
			"./assets/textures/atlas/"+v.AtlasFile,
			v.AtlasID,
			v.Left/atlasWidth,
			v.Top/atlasHeight,
			v.Right/atlasWidth,
			v.Bottom/atlasHeight,
		)
		regionEntries = append(regionEntries, entry)
	}

	for _, seqName := range seqKeys {
		frames := data.SequenceFrames[seqName]
		for _, v := range frames {
			entry := fmt.Sprintf(
				`{"%s",%d,%.8f,%.8f,%.8f,%.8f}`,
				"./assets/textures/atlas/"+v.AtlasFile,
				v.AtlasID,
				v.Left/atlasWidth,
				v.Top/atlasHeight,
				v.Right/atlasWidth,
				v.Bottom/atlasHeight,
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

// toEnumName 将字符串转换为 C++ 枚举名称格式。
func toEnumName(s string) string {
	s = strings.ToLower(s)
	re := strings.NewReplacer("/", "_", ".", "_", "-", "_", " ", "_")
	return re.Replace(s)
}