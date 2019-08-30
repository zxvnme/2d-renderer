#include "../thirdparty/glad/glad.hh"
#include "window.hpp"

Window::Window(const int width, const int height, const std::string_view title, const bool vertical_sync, GLFWmonitor* monitor)
{
    if (!glfwInit())
        std::exit(1);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    this->window_width_ = width;
    this->window_height_ = height;
	this->window_title_ = title.data();

    if (monitor == nullptr) {
        const auto primary_monitor = glfwGetPrimaryMonitor();
        const auto video_mode = glfwGetVideoMode(primary_monitor);

        if (width > video_mode->width)
            this->window_width_ = video_mode->width;
        
        if (height > video_mode->height)
            this->window_height_ = video_mode->height;
    }

	this->window_handle_ = glfwCreateWindow(this->get_width(), this->get_height(), title.data(), monitor, nullptr);

    if (!this->get_window_handle())
        std::exit(1);

    glfwMakeContextCurrent(this->get_window_handle());
    glfwSwapInterval(vertical_sync ? 1 : 0);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
    glfwDestroyWindow(this->window_handle_);
    glfwTerminate();
}

void Window::new_frame()
{
	const auto time_now = glfwGetTime();
	const auto time_delta = time_now - this->previous_time_;

    if (time_delta >= 1.0)
    {
		const auto fps = static_cast<float>(this->frame_count_accumulator_) / time_delta;

		char buffer[128] = { 0 };

		sprintf_s(buffer, "%s - %.1f FPS \n", this->window_title_.data(), fps);

		glfwSetWindowTitle(this->window_handle_, buffer);

		this->frame_count_accumulator_ = 0;
		this->previous_time_ = time_now;
    }

    glfwGetFramebufferSize(this->get_window_handle(), &this->window_width_, &this->window_height_);

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, this->window_width_, this->window_height_);

	this->frame_count_accumulator_++;
}

void Window::finish_frame() const
{
    glfwSwapBuffers(this->get_window_handle());
    glfwPollEvents();
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(this->get_window_handle());
}

int Window::get_width() const
{
    return this->window_width_;
}

int Window::get_height() const
{
    return this->window_height_;
}

GLFWwindow *Window::get_window_handle() const
{
    return this->window_handle_;
}
