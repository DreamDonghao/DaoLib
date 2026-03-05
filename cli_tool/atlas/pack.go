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

const (
	padding     = 2
	atlasWidth  = 2048
	atlasHeight = 2048
)

type packItem struct {
	Rel    string
	Key    string
	Img    image.Image
	W      int
	H      int
	Folder string
}

// makeAtlasJSONPath 从完整路径生成图集JSON路径（仅文件名部分）
func makeAtlasJSONPath(path string) string {
	return filepath.Base(path)
}

// PackAtlas 将PNG图片打包成图集
// rootDir: 输入图片根目录
// outputJSON: 输出元数据JSON文件路径
// outputBase: 输出图集图片基础目录
func PackAtlas(rootDir, outputJSON, outputBase string) error {
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

		folderName := filepath.Base(filepath.Dir(path))

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

		item := packItem{
			Rel:    rel,
			Key:    key,
			Img:    img,
			W:      img.Bounds().Dx(),
			H:      img.Bounds().Dy(),
			Folder: folderName,
		}

		if strings.HasPrefix(folderName, "sf") {
			sequenceGroups[folderName] = append(sequenceGroups[folderName], item)
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

	// 合并序列帧和普通图片
	allItems := append([]packItem{}, imagesToPack...)
	for _, v := range sequenceGroups {
		allItems = append(allItems, v...)
	}

	// 按高度降序排序
	sort.Slice(allItems, func(i, j int) bool {
		return allItems[i].H > allItems[j].H
	})

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
				"atlas_file": makeAtlasJSONPath(atlasName),
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
	if err := os.WriteFile(outputJSON, raw, 0644); err != nil {
		return err
	}

	return nil
}

// drawRect 将源图片绘制到目标图片的指定位置
func drawRect(dst image.Image, src image.Image, x, y int) {
	b := src.Bounds()
	for i := 0; i < b.Dx(); i++ {
		for j := 0; j < b.Dy(); j++ {
			dst.(*image.RGBA).Set(x+i, y+j, src.At(b.Min.X+i, b.Min.Y+j))
		}
	}
}
