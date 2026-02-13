package util

import (
	"regexp"
	"strings"
)

func ToEnumName(s string) string {
	s = strings.ToLower(s)
	re := regexp.MustCompile(`[./\- ]`)
	return re.ReplaceAllString(s, "_")
}
