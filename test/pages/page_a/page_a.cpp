#include <page_a/page_a.hpp>

namespace test {
    std::vector<dao::TextureID> PageA::getRegisterTexture() const {
        return {};
    }

    void PageA::open() {
    }


    void PageA::close() {
    }

    void PageA::update() {
        std::cout << "PageA" << std::endl;
    }

    void PageA::handleInputEvent(const SDL_Event &event) {
    }
}
