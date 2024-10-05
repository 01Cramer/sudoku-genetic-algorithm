#include "Controller.h"

Controller::Controller(uint8_t num_items)
{
	selected_item_index = 0;
	number_of_items = num_items;
	menu = new sf::Text[number_of_items];
};

Controller::Controller() {};

Controller::~Controller()
{
	delete[] menu;
};

void Controller::move_up()
{
	if (selected_item_index - 1 >= 0) {
		menu[selected_item_index].setFillColor(sf::Color::Black);
		selected_item_index--;
		menu[selected_item_index].setFillColor(sf::Color(111, 0, 255));
	}
}

void Controller::move_down()
{
	if (selected_item_index + 1 < number_of_items)
	{
		menu[selected_item_index].setFillColor(sf::Color::Black);
		selected_item_index++;
		menu[selected_item_index].setFillColor(sf::Color(111, 0, 255));
	}
}

uint8_t Controller::get_pressed_item()
{
	return selected_item_index;
}