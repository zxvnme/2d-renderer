#pragma once

#include <string_view>
#include <unordered_map>

#include "../thirdparty/glm/glm.hpp"

class Shader
{
	unsigned int program_id_;
	unsigned int vertex_shader_id_;
	unsigned int fragment_shader_id_;

	std::unordered_map<std::string, int> uniform_location_cache_;
	std::unordered_map<std::string, int> attribute_location_cache_;

	static void compile_shader(unsigned int shader, const std::string &source);

	void load_from_string(const std::string &vertex_shader_source, const std::string &fragment_shader_source) const;
	void load_from_file(const std::string &vertex_shader_file_name, const std::string &fragment_shader_file_name) const;

public:
	explicit Shader(std::string_view shader_name);
	Shader(std::string_view vertex_shader_source, std::string_view fragment_shader_source);

	void bind() const;

	void set(std::string_view name, float value);
	void set(std::string_view name, int value);

	void set(std::string_view name, const glm::vec2 &value);
	void set(std::string_view name, const glm::vec3 &value);
	void set(std::string_view name, const glm::vec4 &value);
	void set(std::string_view name, const glm::mat4 &value);

	int get_uniform_location(std::string_view name);
	int get_attribute_location(std::string_view name);

};
