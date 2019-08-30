#pragma once

#include <GLFW/glfw3.h>
#include <string_view>

class Window {
	int window_width_ = 0;
	int window_height_ = 0;
	std::string window_title_ = "";

	double previous_time_ = 0.0f;
	int frame_count_accumulator_ = 0;

	GLFWwindow* window_handle_ = nullptr;

public:
	Window(int width, int height, std::string_view title, bool vertical_sync = true, GLFWmonitor* monitor = nullptr);
	~Window();

	void new_frame();
	void finish_frame() const;

    bool should_close() const;

    int get_width() const;
    int get_height() const;

    GLFWwindow* get_window_handle() const;

};
