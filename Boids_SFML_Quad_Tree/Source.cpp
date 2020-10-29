#include <iostream>
#include "Application.h"

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Flocking");
    window.setFramerateLimit(30);

	Application application(&window);
    application.run();

	return 0;
}
