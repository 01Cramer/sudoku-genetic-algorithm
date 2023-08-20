#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class UserMode {
public:
	UserMode(float width, float height);
	UserMode();
	~UserMode();
	int draw_user_mode(sf::RenderWindow& window, int sudoku_board[9][9]);
	void move_up();
	void move_down();
	int get_pressed_item();
	std::vector<std::vector<int>> get_board();
	void reset_board();

private:
	int rows = 9;
	int cols = 9;
	std::vector<std::vector<int>> board_to_solve;
	int selected_item_index;
	sf::Font font;
	sf::Text user_mode[2];
};
