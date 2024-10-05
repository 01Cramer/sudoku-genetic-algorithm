#pragma once
#include "Controller.h"

class Menu : public Controller{
public:
	Menu(float width, float height, uint8_t number_of_items);
	~Menu();

	void draw(sf::RenderWindow& window);

private:
	sf::Font font;
};
