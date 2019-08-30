#include "../thirdparty/glad/glad.hh"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "graphics.hpp"

constexpr std::string_view VERTEX_SHADER = R"(
#version 430 core

uniform mat4 projection;

in vec2 position;
in vec4 color;
in vec2 texture_position;

out vec4 _color;
out vec2 _texture_position;

void main()
{
    gl_Position = projection * vec4(position, 0.0, 1.0);

    _color = color;
    _texture_position = texture_position;
}
)";

constexpr std::string_view FRAGMENT_SHADER = R"(
#version 430 core

uniform sampler2D texture;

in vec4 _color;
in vec2 _texture_position;

out vec4 color;

void main()
{
    color = texture2D(texture, _texture_position) * _color;
}
)";

Graphics::Graphics(Window* window)
	: Renderer(new Shader(VERTEX_SHADER, FRAGMENT_SHADER))
{
	this->window_ = window;
	this->simple_shader_ = this->shader_;
	this->white_pixel_ = this->texture_ = new Texture(1, 1, new uint8_t[4]{ 255, 255, 255, 255 });
}

void Graphics::draw_line(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
{
	this->set_vertex_topology(GL_LINES);
	this->allocate_vertices(2);

	this->add_vertex(start, color, glm::vec2(0.0f));
	this->add_vertex(end, color, glm::vec2(0.0f));
}

void Graphics::draw_triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec4& color)
{
	this->draw_line(point1, point2, color);
	this->draw_line(point2, point3, color);
	this->draw_line(point3, point1, color);
}

void Graphics::draw_rectangle(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color)
{
	this->draw_line(min, glm::vec2(max.x, min.y), color);
	this->draw_line(glm::vec2(max.x, min.y), max, color);
	this->draw_line(max, glm::vec2(min.x, max.y), color);
	this->draw_line(glm::vec2(min.x, max.y), min, color);
}

void Graphics::fill_triangle(const glm::vec2& point1, const glm::vec2& point2, const glm::vec2& point3, const glm::vec4& color)
{
	this->set_vertex_topology(GL_TRIANGLES);
	this->allocate_vertices(3);

	this->add_vertex(point1, color, glm::vec2(0.0f, 0.0f));
	this->add_vertex(point2, color, glm::vec2(0.0f, 1.0f));
	this->add_vertex(point3, color, glm::vec2(1.0f, 1.0f));
}

void Graphics::fill_rectangle(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color)
{
	this->fill_triangle(min, glm::vec2(min.x, max.y), max, color);
	this->fill_triangle(min, glm::vec2(max.x, min.y), max, color);
}

void Graphics::display()
{
	if (!this->shader_)
		this->bind_shader(this->simple_shader_);

	if (!this->texture_)
		this->bind_texture(this->white_pixel_);

	this->shader_->set("projection", glm::ortho(0.0f, static_cast<float>(this->window_->get_width()), static_cast<float>(this->window_->get_height()), 0.0f, -1.0f, 1.0f));
	this->shader_->set("texture", 0);

	Renderer::display();
}

void Graphics::clear()
{
	Renderer::clear();
}
