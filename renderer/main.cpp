#include "thirdparty/glm/glm.hpp"

#include "graphics/graphics.hpp"
#include "graphics/window.hpp"

int main()
{
	const auto window = new Window(1280, 720, "Game window", true, nullptr);
	const auto graphics = new Graphics(window);

	while (!window->should_close())
	{
		window->new_frame();

		graphics->fill_rectangle(glm::vec2(16.0f), glm::vec2(80.0f), glm::vec4(1.0f));

		graphics->display();
		graphics->clear();

		window->finish_frame();
	}
}
