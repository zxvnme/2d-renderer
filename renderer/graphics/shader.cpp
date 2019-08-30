#include <fstream>

#include "../thirdparty/glm/gtc/type_ptr.hpp"
#include "../thirdparty/glad/glad.hh"
#include "shader.hpp"

void Shader::compile_shader(const unsigned int shader, const std::string& source)
{
    auto source_cstr = source.c_str();

    glShaderSource(shader, 1, &source_cstr, nullptr);
    glCompileShader(shader);

    int compile_status, log_info_length;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_info_length);

    if (log_info_length > 0)
    {
        std::string log_message;

        log_message.reserve(log_info_length + 1);

        glGetShaderInfoLog(shader, log_info_length, nullptr, &log_message[0]);

        printf("%s\n", log_message.c_str());
    }
}

void Shader::load_from_string(const std::string& vertex_shader_source, const std::string& fragment_shader_source) const
{
    this->compile_shader(this->vertex_shader_id_, vertex_shader_source);
    this->compile_shader(this->fragment_shader_id_, fragment_shader_source);

    glAttachShader(this->program_id_, this->vertex_shader_id_);
    glAttachShader(this->program_id_, this->fragment_shader_id_);

    glLinkProgram(this->program_id_);

    int program_link_status, program_log_info_length;

    glGetProgramiv(this->program_id_, GL_LINK_STATUS, &program_link_status);
    glGetProgramiv(this->program_id_, GL_INFO_LOG_LENGTH, &program_log_info_length);

    if (program_log_info_length > 0)
    {
        std::string log_message;

        log_message.reserve(program_log_info_length + 1);

        glGetProgramInfoLog(this->program_id_, program_log_info_length, nullptr, &log_message[0]);

        printf("%s\n", log_message.c_str());
    }
}

void Shader::load_from_file(const std::string &vertex_shader_file_name, const std::string &fragment_shader_file_name) const
{
    std::ifstream vertex_shader_stream(vertex_shader_file_name, std::ios::ate);
    std::ifstream fragment_shader_stream(fragment_shader_file_name, std::ios::ate);

    const auto vertex_shader_length = vertex_shader_stream.tellg(), fragment_shader_length = fragment_shader_stream.tellg();
    std::string vertex_shader, fragment_shader;

    vertex_shader.resize(static_cast<int>(vertex_shader_length) + 1);
    fragment_shader.resize(static_cast<int>(fragment_shader_length) + 1);

    vertex_shader_stream.seekg(0, std::ios::beg);
    vertex_shader_stream.read(&vertex_shader[0], vertex_shader_length);
    vertex_shader_stream.close();

    fragment_shader_stream.seekg(0, std::ios::beg);
    fragment_shader_stream.read(&fragment_shader[0], fragment_shader_length);
    fragment_shader_stream.close();

    this->load_from_string(vertex_shader, fragment_shader);
}

Shader::Shader(const std::string_view shader_name)
{
    this->program_id_ = glCreateProgram();
    this->vertex_shader_id_ = glCreateShader(GL_VERTEX_SHADER);
    this->fragment_shader_id_ = glCreateShader(GL_FRAGMENT_SHADER);

    this->load_from_file(std::string(shader_name) + ".vs", std::string(shader_name) + ".fs");
}

Shader::Shader(const std::string_view vertex_shader_source, const std::string_view fragment_shader_source)
{
    this->program_id_ = glCreateProgram();
    this->vertex_shader_id_ = glCreateShader(GL_VERTEX_SHADER);
    this->fragment_shader_id_ = glCreateShader(GL_FRAGMENT_SHADER);

    this->load_from_string(vertex_shader_source.data(), fragment_shader_source.data());
}

void Shader::bind() const
{
    glUseProgram(this->program_id_);
}

void Shader::set(const std::string_view name, const float value)
{
    const auto uniform_location = this->get_uniform_location(name);

    if (uniform_location == -1)
        return;

    glUniform1f(uniform_location, value);
}

void Shader::set(const std::string_view name, const int value)
{
    const auto uniform_location = this->get_uniform_location(name);

    if (uniform_location == -1)
        return;

    glUniform1i(uniform_location, value);
}

void Shader::set(const std::string_view name, const glm::vec2 &value)
{
    const auto uniform_location = this->get_uniform_location(name);

    if (uniform_location == -1)
        return;

    glUniform2f(uniform_location, value.x, value.y);
}

void Shader::set(const std::string_view name, const glm::vec3 &value)
{
    const auto uniform_location = this->get_uniform_location(name);

    if (uniform_location == -1)
        return;

    glUniform3f(uniform_location, value.x, value.y, value.z);
}

void Shader::set(const std::string_view name, const glm::vec4 &value)
{
    const auto uniform_location = this->get_uniform_location(name);

    if (uniform_location == -1)
        return;

    glUniform4f(uniform_location, value.x, value.y, value.z, value.w);
}

void Shader::set(const std::string_view name, const glm::mat4 &value)
{
    const auto uniform_location = this->get_uniform_location(name);

    if (uniform_location == -1)
        return;

    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, value_ptr(value));
}

int Shader::get_uniform_location(const std::string_view name)
{
    if (this->uniform_location_cache_.find(name.data()) == this->uniform_location_cache_.end())
    {
        const auto uniform_location = glGetUniformLocation(this->program_id_, name.data());

        this->uniform_location_cache_.insert({ name.data(), uniform_location });

        return uniform_location;
    }

    return this->uniform_location_cache_.at(name.data());
}

int Shader::get_attribute_location(const std::string_view name)
{
    if (this->attribute_location_cache_.find(name.data()) == this->attribute_location_cache_.end())
    {
        const auto attribute_location = glGetAttribLocation(this->program_id_, name.data());

        this->attribute_location_cache_.insert({ name.data(), attribute_location });

        return attribute_location;
    }

    return this->attribute_location_cache_.at(name.data());
}
