#pragma once

#include <vector>
#include "../thirdparty/glm/glm.hpp"

#include "shader.hpp"
#include "texture.hpp"

struct Batch
{
	Shader* associated_shader;
	Texture* associated_texture;

	int vertex_topology;
	int vertex_count;

	Batch(Shader* shader, Texture* texture, const int vertex_topology, const int vertex_count)
		: associated_shader(shader)
		, associated_texture(texture)
		, vertex_topology(vertex_topology)
		, vertex_count(vertex_count)
	{
		// Initialized using initializer list
	}

};

struct Vertex
{
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 texture_position;

	Vertex(const glm::vec2& position, const glm::vec4& color, const glm::vec2& texture_position)
		: position(position)
		, color(color)
		, texture_position(texture_position)
	{
		// Initialized using initializer list
	}

};

class Renderer
{
	unsigned int vertex_array_;
	unsigned int vertex_buffer_;

	int vertex_topology_;
	bool data_changed_;

	std::vector<Vertex> vertices_;
	std::vector<Batch> batches_;

protected:
	Texture* texture_;
	Shader *shader_;

	void manage_batches();

public:
	explicit Renderer(Shader* shader);

	void set_vertex_topology(int vertex_topology);

	void allocate_vertices(int count);
	void add_vertices(Vertex* vertices, int count);

	void add_vertex(const Vertex& vertex);
	void add_vertex(const glm::vec2& position, const glm::vec4& color, const glm::vec2& texture_position);

	virtual void display();
	virtual void clear();

	void bind_texture(Texture* texture);
	void bind_shader(Shader* shader);

};
