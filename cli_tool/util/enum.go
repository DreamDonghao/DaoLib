package util

import (
	"regexp"
	"strings"
)

// ToEnumName 将字符串转换为 C++ 枚举名称格式
func ToEnumName(s string) string {
	s = strings.ToLower(s)
	re := regexp.MustCompile(`[./\- ]`)
	return re.ReplaceAllString(s, "_")
}
