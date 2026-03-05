package cmdRun

import (
	"fmt"
	"os"
)

func CreateTray() error {
	hpp := `#pragma once
#include <core/frame/Tray.hpp>
#include <core/frame/Context.hpp>

class AppTray : public dao::Tray {
public:
    AppTray(const std::string_view iconPath, const std::string_view tooltip)
        : Tray(iconPath, tooltip) {
    }

    void createTrayMenu() override;

private:
    dao::TrayFunc example = [](SDL_TrayEntry *entry) {
        std::cout<<"托盘按钮"<<std::endl;
    };
};
`
	cpp := `#include <AppTray.hpp>

void AppTray::createTrayMenu() {
    button("样例", example);
}
`
	if err := os.MkdirAll("./tray", 0755); err != nil {
		fmt.Println("创建目录失败:", err)
		return err
	}
	hppPath := "./tray/" + "AppTray.hpp"
	cppPath := "./tray/" + "AppTray.cpp"
	if err := os.WriteFile(hppPath, []byte(hpp), 0644); err != nil {
		fmt.Println("写入 hpp 文件失败:", err)
		return err
	}
	if err := os.WriteFile(cppPath, []byte(cpp), 0644); err != nil {
		fmt.Println("写入 cpp 文件失败:", err)
		return err
	}
	return nil
}
