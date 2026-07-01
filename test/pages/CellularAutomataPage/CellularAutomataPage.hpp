#pragma once
#include "components/graphs/Rectangle.hpp"
#include "core/frame/FrameLimiter.hpp"
#include "interface/IGeneralPage.hpp"
#include <core/tool/TickTimer.hpp>

#include "components/RectTextBox.hpp"
#include "components/controls/InputBox.hpp"
#include "components/controls/SimpleButton.hpp"
namespace test {
    class CellularAutomataPage:public dao::ifc::IGeneralPage {
        bool isStop = true;
        dao::TickTimer timer{std::chrono::milliseconds(300)};
        dao::u32 wordSize{200};
        std::vector<std::vector<dao::i32>> words{wordSize,std::vector<dao::i32>(wordSize)};
        std::vector<dao::i32> survival{2,3};
        std::vector<dao::i32> death{0,1,4,5,6,7,8};
        std::vector<dao::i32> birth{3};
        dao::InputBox inputBox1{100,25,100,36};
        dao::InputBox inputBox2{100,25+50,100,36};
        dao::InputBox inputBox3{100,25+100,100,36};
        dao::Button begin{25,200,36*2,36,[this] {
            timer.reset();
            isStop = false;
        }};
        dao::RectTextBox beginTextBox{25,200,72,36,36,dao::ColorRGBA("#FFFFFF"),dao::ColorRGBA("#181A1D"),U"开始"};
        dao::Button randomFill{25,300,36*2,36,[this]() {
            for (dao::i32 i = 0;i < words.size();++i) {
                for (dao::i32 j = 0;j < words[i].size();++j) {
                    if (dao::randomInt(0,2) == 1) {
                        words[i][j] = 1;
                    };
                }
            }
        }};
        dao::RectTextBox randomFillTextBox{25,300,72*2,36,36,dao::ColorRGBA("#FFFFFF"),dao::ColorRGBA("#181A1D"),U"随机填充"};
    public:
        CellularAutomataPage():IGeneralPage("CellularAutomataPage"){}

        void open() override;

        void close() override;

        [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override;

        void update() override;

        void handleInputEvent(const SDL_Event &event) override;
    };
}
