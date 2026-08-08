#include "CellularAutomataPage.hpp"

#include "components/RectTextBox.hpp"
#include "components/graphs/Line.hpp"
#include "components/controls/InputBox.hpp"

namespace test {
    void CellularAutomataPage::open() {
        words[1][2] = 1;
        words[1][3] = 1;
        words[2][2] = 1;
        words[2][3] = 1;
    }

    void CellularAutomataPage::close() {
    }

    std::vector<dao::i32> CellularAutomataPage::getRegisterTextures() const {
        return {};
    }

    dao::Rectangle rect1{400, 0, 800, 800, dao::ColorRGBA("#25282C")};
    dao::Rectangle wordBackGround{425, 25, 750, 750, dao::White};
    dao::Rectangle rect{0, 0, 0, 0};
    dao::Line line{0, 0, 0, 0, 1, dao::ColorRGBA("#25282C")};

    dao::RectTextBox textBox1{
        25, 25, 36 * 2, 36, 36, dao::Blue, dao::Black, U"生存"
    };
    dao::RectTextBox textBox2{
        25, 25 + 50, 36 * 2, 36, 36, dao::Blue, dao::Black, U"死亡"
    };
    dao::RectTextBox textBox3{
        25, 25 + 50 * 2, 36 * 2, 36, 36, dao::Blue, dao::Black, U"诞生"
    };

    void CellularAutomataPage::update() {
        addToBatch(
            rect1, wordBackGround, textBox1, textBox2, textBox3,
            inputBox1, inputBox2, inputBox3,
            begin, randomFill,
            beginTextBox,randomFillTextBox
        );
        dao::f32 width = 750.0f / static_cast<dao::f32>(wordSize);

        if (!isStop && timer) {
            const auto tmp = words;
            for (dao::i32 i = 0; i < words.size(); ++i) {
                for (dao::i32 j = 0; j < words[i].size(); ++j) {
                    dao::i32 count = 0;
                    for (const auto mi: {i - 1, i, i + 1}) {
                        for (const auto mj: {j - 1, j, j + 1}) {
                            if ((i == mi && j == mj)
                                || mi < 0 || mi >= words.size() || mj < 0 || mj >= words[i].size()) {
                                continue;
                            }
                            count += tmp[mi][mj] == 1;
                        }
                    }
                    if (words[i][j] == 1 && std::ranges::contains(survival, count)) {
                        words[i][j] = 1;
                    } else if (words[i][j] == 1 && std::ranges::contains(death, count)) {
                        words[i][j] = 0;
                    } else if (words[i][j] == 0 && std::ranges::contains(birth, count)) {
                        words[i][j] = 1;
                    }
                }
            }
        }
        for (dao::i32 i = 0; i < words.size(); ++i) {
            for (dao::i32 j = 0; j < words[i].size(); ++j) {
                if (words[i][j] == 0) {
                    rect.setColor(dao::Black);
                } else if (words[i][j] == 1) {
                    rect.setColor(dao::White);
                }
                rect.setSize(width, width);
                rect.setPosition(425.0f + static_cast<dao::f32>(i) * width, 25.0f + static_cast<dao::f32>(j) * width);
                addToBatch(rect);
            }
        }
        for (dao::i32 i = 1; i < wordSize; ++i) {
            line.setStart(425 + static_cast<dao::f32>(i) * width, 25);
            line.setEnd(425 + static_cast<dao::f32>(i) * width, 750 + 25);
            addToBatch(line);
            line.setStart(425, 25.0f + static_cast<dao::f32>(i) * width);
            line.setEnd(425.0f + 750.f, 25.0f + static_cast<dao::f32>(i) * width);
            addToBatch(line);
        }
    }

    void CellularAutomataPage::handleInputEvent(const SDL_Event &event) {
        inputBox1.handleEvent(event);
        inputBox2.handleEvent(event);
        inputBox3.handleEvent(event);
        begin.handleEvent(event);
        randomFill.handleEvent(event);
    }
}
