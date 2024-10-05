#pragma once
#include <SFML/Graphics.hpp>
#include "Controller.h"

class Demo : public Controller{
public:
	Demo(float width, float height, uint8_t number_of_items);
	~Demo();

	void draw(sf::RenderWindow& window);
	void draw_mode(sf::RenderWindow& window, int board[9][9]);

	// Navigating demo menu functions
	void move_up(bool modes);
	void move_down(bool modes);
	int get_action();

private:
	int action;
	sf::Font font;
	sf::Text solve_or_back[2];
};
