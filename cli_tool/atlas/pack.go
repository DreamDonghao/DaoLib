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
	PADDING      = 2
	ATLAS_WIDTH  = 2048
	ATLAS_HEIGHT = 2048
)

type packItem struct {
	Rel    string
	Key    string
	Img    image.Image
	W      int
	H      int
	Folder string
}

func makeAtlasJSONPath(path string) string {
	return filepath.Base(path)
}

func PackAtlas(rootDir, outputJSON, outputBase string) error {

	fmt.Println("扫描 PNG:", rootDir)

	var imagesToPack []packItem
	sequenceGroups := map[string][]packItem{}

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
		return fmt.Errorf("没有 PNG")
	}

	// 合并
	allItems := append([]packItem{}, imagesToPack...)
	for _, v := range sequenceGroups {
		allItems = append(allItems, v...)
	}

	// 按高度排序
	sort.Slice(allItems, func(i, j int) bool {
		return allItems[i].H > allItems[j].H
	})

	atlasIndex := 2
	cur := 0

	imageMeta := map[string]interface{}{}
	sequenceMeta := map[string][]interface{}{}

	for name := range sequenceGroups {
		sequenceMeta[name] = []interface{}{}
	}

	for cur < len(allItems) {

		atlasFileName := fmt.Sprintf("atlas_%04d.png", atlasIndex)
		atlasName := filepath.Join(outputBase, atlasFileName)

		atlas := image.NewRGBA(image.Rect(0, 0, ATLAS_WIDTH, ATLAS_HEIGHT))

		x, y, rowH := 0, 0, 0
		i := cur

		for i < len(allItems) {
			item := allItems[i]
			w, h := item.W, item.H

			newLine := x+w > ATLAS_WIDTH
			ny := y
			if newLine {
				ny = y + rowH + PADDING
			}

			if ny+h > ATLAS_HEIGHT {
				break
			}

			if newLine {
				x = 0
				y = ny
				rowH = 0
			}

			drawRect(atlas, item.Img, x, y)

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

			x += w + PADDING
			if h > rowH {
				rowH = h
			}
			i++
		}

		out, _ := os.Create(atlasName)
		png.Encode(out, atlas)
		out.Close()

		atlasIndex++
		cur = i
	}

	final := map[string]interface{}{
		"images":          imageMeta,
		"sequence_frames": sequenceMeta,
	}

	raw, _ := json.MarshalIndent(final, "", "  ")
	os.WriteFile(outputJSON, raw, 0644)

	return nil
}

func drawRect(dst image.Image, src image.Image, x, y int) {
	b := src.Bounds()
	for i := 0; i < b.Dx(); i++ {
		for j := 0; j < b.Dy(); j++ {
			dst.(*image.RGBA).Set(x+i, y+j, src.At(b.Min.X+i, b.Min.Y+j))
		}
	}
}
