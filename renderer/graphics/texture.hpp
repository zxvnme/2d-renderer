#pragma once

#include <string_view>

class Texture
{
    unsigned int texture_id_;

    int width_;
    int height_;
    int channel_count_;

public:
    Texture(int width, int height, uint8_t* data);
    explicit Texture(std::string_view filename);

    void bind() const;

    unsigned int get_id() const;

};
