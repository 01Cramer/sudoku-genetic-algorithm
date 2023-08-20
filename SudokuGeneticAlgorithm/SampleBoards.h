#pragma once
#include <vector>
class SampleBoards {
public:
	int clear_board[9][9];
	int easy_board[9][9];
	int hard_board[9][9];

	std::vector<std::vector<int>> easy_vector;
	std::vector<std::vector<int>> hard_vector;

	void clear();

	SampleBoards();
};
