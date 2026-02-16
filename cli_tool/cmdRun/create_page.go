package cmdRun

import (
	"fmt"
	"os"
	"strings"
	"unicode"
)

// PascalCase → snake_case
func toSnakeCase(s string) string {
	var result []rune
	for i, r := range s {
		if unicode.IsUpper(r) {
			if i > 0 {
				result = append(result, '_')
			}
			result = append(result, unicode.ToLower(r))
		} else {
			result = append(result, r)
		}
	}
	return string(result)
}

func CreatePage(name string) {
	// 解析 namespace 和 pageName
	idx := strings.LastIndex(name, "::")

	var namespace string
	var pageName string
	if idx == -1 {
		pageName = name
	} else {
		namespace = name[:idx]
		pageName = name[idx+2:]
	}

	// 文件名使用 snake_case
	fileBase := toSnakeCase(pageName)

	// 输出目录：pages/类名(下划线)
	dir := "./pages/" + fileBase

	if err := os.MkdirAll(dir, 0755); err != nil {
		fmt.Println("Error creating directory:", err)
		return
	}

	// include 路径
	includePath := fileBase + "/" + fileBase + ".hpp"

	// hpp
	hpp := []byte(fmt.Sprintf(`#pragma once
#include <interface/general_page.hpp>

namespace %s {
    class %s : public dao::GeneralPage {
    public:
        %s() : GeneralPage("setPage") {
        };

        ~%s() override = default;

        [[nodiscard]] std::vector<dao::uint32> getRegisterTexture() const override;

        void init() override;

        void close() override;

        void update() override;

        void handleInputEvent(const SDL_Event &event) override;
    };
}`, namespace, pageName, pageName, pageName))

	hppPath := dir + "/" + fileBase + ".hpp"
	if err := os.WriteFile(hppPath, hpp, 0644); err != nil {
		fmt.Println("Error writing hpp:", err)
		return
	}

	// cpp
	cpp := []byte(fmt.Sprintf(`#include <%s>

namespace %s {
    std::vector<dao::uint32> %s::getRegisterTexture() const {
        return {};
    }

    void %s::init() {
    }

    void %s::close() {
    }

    void %s::update() {
    }

    void %s::handleInputEvent(const SDL_Event &event) {
    }
}`, includePath, namespace, pageName, pageName, pageName, pageName, pageName))

	cppPath := dir + "/" + fileBase + ".cpp"
	if err := os.WriteFile(cppPath, cpp, 0644); err != nil {
		fmt.Println("Error writing cpp:", err)
		return
	}
}
