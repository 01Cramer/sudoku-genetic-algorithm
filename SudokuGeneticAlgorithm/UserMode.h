#pragma once
#include <SFML/Graphics.hpp>
#include "Controller.h"
#include <string>
#include <vector>
#include <Windows.h>

class UserMode : public Controller{
public:
	UserMode(float width, float height, uint8_t number_of_items);
	UserMode();
	~UserMode();
	int draw_user_mode(sf::RenderWindow& window, int sudoku_board[9][9], std::string board_encoded);
	std::vector<std::vector<int>> get_board();
	void reset_board();
	bool validate_board();
	int display_warning_box();

private:
	int rows = 9;
	int cols = 9;
	std::vector<std::vector<int>> board_to_solve;
	sf::Font font;
};
