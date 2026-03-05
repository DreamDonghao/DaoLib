package cmdRun

import (
	"fmt"
	"image"
	"image/draw"
	"image/gif"
	"image/png"
	"os"
	"path/filepath"
)

// RunGif2Png 将 GIF 文件转换为 PNG 序列帧
func RunGif2Png(args []string) {
	if len(args) < 2 {
		fmt.Println("用法: dao gif2png input.gif output_dir")
		return
	}

	inputPath := args[0]
	outputDir := args[1]

	if err := os.MkdirAll(outputDir, os.ModePerm); err != nil {
		fmt.Println("创建输出目录失败:", err)
		return
	}

	file, err := os.Open(inputPath)
	if err != nil {
		fmt.Println("打开 GIF 失败:", err)
		return
	}
	defer file.Close()

	g, err := gif.DecodeAll(file)
	if err != nil {
		fmt.Println("解析 GIF 失败:", err)
		return
	}

	canvas := image.NewRGBA(g.Image[0].Bounds())

	for i, frame := range g.Image {
		draw.Draw(canvas, frame.Bounds(), frame, image.Point{}, draw.Over)

		outPath := filepath.Join(outputDir, fmt.Sprintf("frame_%03d.png", i))
		outFile, err := os.Create(outPath)
		if err != nil {
			fmt.Println("创建文件失败:", err)
			return
		}

		if err := png.Encode(outFile, canvas); err != nil {
			outFile.Close()
			fmt.Println("写入 PNG 失败:", err)
			return
		}
		outFile.Close()

		fmt.Println("导出:", outPath)
	}

	fmt.Printf("完成，共导出 %d 帧\n", len(g.Image))
}
