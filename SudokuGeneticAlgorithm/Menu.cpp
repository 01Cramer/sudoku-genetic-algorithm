#include "Menu.h"


Menu::Menu(float width, float height) {
	if (!font.loadFromFile("Alice-Regular.ttf")) {
		throw std::exception("Cant find font!");
	}

	menu[0].setFont(font);
	menu[0].setCharacterSize(64);
	menu[0].setFillColor(sf::Color(111, 0, 255));
	menu[0].setString("Solve Your's Sudoku");
	menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));
	sf::FloatRect textRect = menu[0].getLocalBounds();
	menu[0].move(-textRect.width / 2.0f, 0); // SFML operates on floats

	menu[1].setFont(font);
	menu[1].setCharacterSize(64);
	menu[1].setFillColor(sf::Color::Black);
	menu[1].setString("Demonstration");
	menu[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));
	textRect = menu[1].getLocalBounds();
	menu[1].move(-textRect.width / 2.0f, 0);

	menu[2].setFont(font);
	menu[2].setCharacterSize(64);
	menu[2].setFillColor(sf::Color::Black);
	menu[2].setString("Exit");
	menu[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));
	textRect = menu[2].getLocalBounds();
	menu[2].move(-textRect.width / 2.0f, 0);

	selected_item_index = 0;
}

Menu :: ~Menu() {}

void Menu::draw(sf::RenderWindow& window) {
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {
		window.draw(menu[i]);
	}
}

void Menu::move_up() {
	if (selected_item_index - 1 >= 0) {
		menu[selected_item_index].setFillColor(sf::Color::Black);
		selected_item_index--;
		menu[selected_item_index].setFillColor(sf::Color(111, 0, 255));
	}
}

void Menu::move_down() {
	if (selected_item_index + 1 <= 2) {
		menu[selected_item_index].setFillColor(sf::Color::Black);
		selected_item_index++;
		menu[selected_item_index].setFillColor(sf::Color(111, 0, 255));
	}
}

int Menu::get_pressed_item() {
	return selected_item_index;
}