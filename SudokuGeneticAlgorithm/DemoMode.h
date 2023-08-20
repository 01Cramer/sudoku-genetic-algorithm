#pragma once
#include <SFML/Graphics.hpp>
#define MAX_NUMBER_OF_ITEMS_DEMO 4

class Demo {
public:
	Demo(float width, float height);
	~Demo();

	void draw(sf::RenderWindow& window);
	void draw_mode(sf::RenderWindow& window, int board[9][9]);

	// Navigating the demo mode menu functions
	void move_up(bool modes);
	void move_down(bool modes);
	int get_pressed_item();
	int get_action();

private:
	int selected_item_index;
	int action;
	sf::Font font;
	sf::Text demo[MAX_NUMBER_OF_ITEMS_DEMO];
	sf::Text solve_or_back[2];
};
