package atlas

import (
	"encoding/json"
	"os"
)

type FrameMeta struct {
	AtlasFile string  `json:"atlas_file"`
	AtlasID   int     `json:"atlas_id"`
	Left      float64 `json:"left"`
	Top       float64 `json:"top"`
	Right     float64 `json:"right"`
	Bottom    float64 `json:"bottom"`
}

type AtlasMeta struct {
	Images         map[string]FrameMeta   `json:"images"`
	SequenceFrames map[string][]FrameMeta `json:"sequence_frames"`
}

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
