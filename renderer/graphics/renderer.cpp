#include "renderer.hpp"
#include "../thirdparty/glad/glad.hh"

// ReSharper disable once CppPossiblyUninitializedMember
Renderer::Renderer(Shader* shader)
{
	this->shader_ = shader;

	glGenVertexArrays(1, &this->vertex_array_);
	glBindVertexArray(this->vertex_array_);

	glGenBuffers(1, &this->vertex_buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_);

	glEnableVertexAttribArray(this->shader_->get_attribute_location("position"));
	glVertexAttribPointer(this->shader_->get_attribute_location("position"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<Vertex*>(nullptr)->position);

	glEnableVertexAttribArray(this->shader_->get_attribute_location("color"));
	glVertexAttribPointer(this->shader_->get_attribute_location("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<Vertex*>(nullptr)->color);

	glEnableVertexAttribArray(this->shader_->get_attribute_location("texture_position"));
	glVertexAttribPointer(this->shader_->get_attribute_location("texture_position"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<Vertex*>(nullptr)->texture_position);
}

void Renderer::manage_batches()
{
	if (this->batches_.empty())
	{
		this->batches_.emplace_back(Batch(this->shader_, this->texture_, this->vertex_topology_, 0));

		return;
	}

	const auto last_batch = this->batches_.back();

	if (last_batch.vertex_topology != this->vertex_topology_ || last_batch.associated_shader != this->shader_ || last_batch.associated_texture != this->texture_)
		this->batches_.emplace_back(Batch(this->shader_, this->texture_, this->vertex_topology_, 0));
}

void Renderer::set_vertex_topology(const int vertex_topology)
{
	this->vertex_topology_ = vertex_topology;
}

void Renderer::allocate_vertices(const int count)
{
	this->vertices_.reserve(count);
}

void Renderer::add_vertices(Vertex* vertices, const int count)
{
	this->manage_batches();

	this->data_changed_ = true;
	this->batches_.back().vertex_count += count;

	this->vertices_.reserve(count);

	for (auto i = 0; i < count; i++)
		this->vertices_.push_back(vertices[i]);
}

void Renderer::add_vertex(const Vertex& vertex)
{
	auto vtx = vertex;

	this->add_vertices(&vtx, 1);
}

void Renderer::add_vertex(const glm::vec2& position, const glm::vec4& color, const glm::vec2& texture_position)
{
	this->add_vertex(Vertex(position, color, texture_position));
}

void Renderer::display()
{
	glBindVertexArray(this->vertex_array_);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_);

	if (this->data_changed_)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices_.size(), this->vertices_.data(), GL_STATIC_DRAW);

		this->data_changed_ = false;
	}

	auto offset = 0;

	for (const auto& batch : this->batches_)
	{
		if (batch.associated_shader)
			batch.associated_shader->bind();

		if (batch.associated_texture)
			batch.associated_texture->bind();

		glDrawArrays(GL_TRIANGLES, offset, batch.vertex_count);

		offset += batch.vertex_count;
	}
}

void Renderer::clear()
{
	this->batches_.clear();
	this->vertices_.clear();

	this->manage_batches();
}

void Renderer::bind_texture(Texture* texture)
{
	this->texture_ = texture;
}

void Renderer::bind_shader(Shader* shader)
{
	this->shader_ = shader;
}
