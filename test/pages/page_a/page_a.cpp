#include <page_a/page_a.hpp>
#include <components/graphs/Circle.hpp>
#include <components/graphs/Ellipse.hpp>
#include <components/graphs/Rectangle.hpp>
#include <components/graphs/Triangle.hpp>
#include <components/graphs/Line.hpp>
#include <components/graphs/Arc.hpp>
#include <components/graphs/Sector.hpp>
#include <components/graphs/Ring.hpp>
#include <components/graphs/RoundedRectangle.hpp>
#include <cmath>

using namespace dao;

namespace test {
    // 颜色定义
    constexpr ColorRGBA ColCircle(255, 100, 100, 1.0f);        // 红色
    constexpr ColorRGBA ColEllipse(100, 255, 100, 1.0f);       // 绿色
    constexpr ColorRGBA ColRectangle(100, 100, 255, 1.0f);     // 蓝色
    constexpr ColorRGBA ColTriangle(255, 255, 100, 1.0f);      // 黄色
    constexpr ColorRGBA ColLine(255, 100, 255, 1.0f);          // 洋红
    constexpr ColorRGBA ColArc(100, 255, 255, 1.0f);           // 青色
    constexpr ColorRGBA ColSector(255, 200, 100, 1.0f);        // 橙色
    constexpr ColorRGBA ColRing(200, 100, 255, 1.0f);          // 紫色
    constexpr ColorRGBA ColRounded(150, 200, 150, 1.0f);       // 灰绿

    // 图形实例
    Circle circle(100, 100, 40, ColCircle);
    Ellipse ellipse(250, 100, 50, 30, ColEllipse);
    Rectangle rect(320, 60, 80, 80, ColRectangle);
    Triangle triangle(450, 60, 500, 140, 400, 140, ColTriangle);
    Line line(520, 60, 600, 140, 4.0f, ColLine);
    Arc arc(100, 250, 40, 0.0f, f32pi, 6.0f, ColArc);
    Sector sector(250, 250, 40, 0.0f, f32pi * 1.5f, ColSector);
    Ring ring(400, 250, 25, 45, ColRing);
    RoundedRectangle roundedRect(480, 210, 100, 80, 15, ColRounded);

    std::vector<i32> PageA::getRegisterTextures() const {
        return {};
    }

    void PageA::open() {
        m_time = 0.0f;
    }

    void PageA::close() {
    }

    void PageA::update() {
        m_time += 0.02f;

        clearBatch();

        // Circle - 平移动画
        circle.setPosition(100 + std::sin(m_time) * 30, 100);

        // Ellipse - 缩放动画
        ellipse.setRadius(50 + std::sin(m_time) * 10, 30 + std::cos(m_time) * 5);

        // Rectangle - 颜色渐变
        rect.setColor(ColorRGBA(
            static_cast<i32>(127 + 127 * std::sin(m_time)),
            100,
            static_cast<i32>(127 + 127 * std::cos(m_time)),
            1.0f));

        // Triangle - 旋转动画
        triangle.rotate(450, 100, 0.01f);

        // Line - 厚度动画
        line.setThickness(2.0f + std::abs(std::sin(m_time)) * 6.0f);

        // Arc - 角度变化
        arc.setAngles(m_time, m_time + f32pi);

        // Sector - 角度变化
        sector.setAngles(m_time, m_time + f32pi * 1.5f);

        // Ring - 内外半径变化
        ring.setInnerRadius(20 + std::sin(m_time) * 10);
        ring.setOuterRadius(40 + std::cos(m_time) * 10);

        // RoundedRectangle - 圆角变化
        roundedRect.setCornerRadius(5 + std::abs(std::sin(m_time)) * 20);

        // 添加到批处理
        addToBatch(circle);
        addToBatch(ellipse);
        addToBatch(rect);
        addToBatch(triangle);
        addToBatch(line);
        addToBatch(arc);
        addToBatch(sector);
        addToBatch(ring);
        addToBatch(roundedRect);
    }

    void PageA::handleInputEvent(const SDL_Event &event) {
    }
}