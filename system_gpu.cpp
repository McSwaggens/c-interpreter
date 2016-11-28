#include "system_gpu.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdlib>

sf::RenderWindow* window;

void SystemGPU::Init ()
{
	window = new sf::RenderWindow (sf::VideoMode(3840, 2160), "VC");
}

void SystemGPU::Display ()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
			exit (0); // Exit the process with EXIT_SUCCESS code (0)
		}
	}
	
	window->display ();
}

void SystemGPU::Clear (char* color)
{
	window->clear (*(sf::Color*)(void*)color);
}