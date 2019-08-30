#pragma once

#include "../thirdparty/glm/glm.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "window.hpp"

class Graphics : public Renderer
{
    Shader* simple_shader_;
    Texture* white_pixel_;
    Window* window_;

public:
    explicit Graphics(Window* window);

    void draw_line(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
    void draw_triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec4& color);
    void draw_rectangle(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color);

    void fill_triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec4& color);
    void fill_rectangle(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color);

    void display() override;
    void clear() override;

};
