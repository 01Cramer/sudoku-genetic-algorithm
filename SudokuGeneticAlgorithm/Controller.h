#pragma once
#include <SFML/Graphics.hpp>

class Controller
{
protected:
	uint8_t number_of_items;
	uint8_t selected_item_index;
	sf::Text* menu;

public:
	Controller(uint8_t num_items);
	Controller();
	~Controller();

public:
	// Navigating menu - default functions
	void move_up();
	void move_down();
	uint8_t get_pressed_item();
};
