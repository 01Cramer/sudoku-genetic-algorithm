#include "SampleBoards.h"

SampleBoards::SampleBoards() : easy_vector(9, std::vector<int>(9)), hard_vector(9, std::vector<int>(9)) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			clear_board[i][j] = 0;
			easy_board[i][j] = 0;
			hard_board[i][j] = 0;
		}
	}

	/// Arrays ///

	easy_board[0][1] = 8;
	easy_board[0][7] = 9;
	easy_board[1][6] = 8;
	easy_board[1][2] = 7;
	easy_board[1][3] = 5;
	easy_board[1][5] = 2;
	easy_board[2][0] = 6;
	easy_board[2][3] = 8;
	easy_board[2][5] = 7;
	easy_board[2][8] = 5;
	easy_board[3][0] = 3;
	easy_board[3][1] = 7;
	easy_board[3][4] = 8;
	easy_board[3][7] = 5;
	easy_board[3][8] = 1;
	easy_board[4][0] = 2;
	easy_board[4][8] = 8;
	easy_board[5][0] = 9;
	easy_board[5][1] = 5;
	easy_board[5][4] = 4;
	easy_board[5][7] = 3;
	easy_board[5][8] = 2;
	easy_board[6][0] = 8;
	easy_board[6][3] = 1;
	easy_board[6][5] = 4;
	easy_board[6][8] = 9;
	easy_board[7][2] = 1;
	easy_board[7][3] = 9;
	easy_board[7][5] = 3;
	easy_board[7][6] = 6;
	easy_board[8][1] = 4;
	easy_board[8][7] = 2;

	hard_board[0][2] = 6;
	hard_board[1][1] = 8;
	hard_board[1][4] = 5;
	hard_board[1][5] = 4;
	hard_board[1][6] = 2;
	hard_board[2][1] = 4;
	hard_board[2][4] = 9;
	hard_board[2][7] = 7;
	hard_board[3][2] = 7;
	hard_board[3][3] = 9;
	hard_board[3][6] = 3;
	hard_board[4][4] = 8;
	hard_board[4][6] = 4;
	hard_board[5][0] = 6;
	hard_board[5][6] = 1;
	hard_board[6][0] = 2;
	hard_board[6][2] = 3;
	hard_board[6][8] = 1;
	hard_board[7][3] = 5;
	hard_board[7][7] = 4;
	hard_board[8][2] = 8;
	hard_board[8][3] = 3;
	hard_board[8][6] = 5;
	hard_board[8][8] = 2;

	/// Vectors ///

	easy_vector[0][1] = 8;
	easy_vector[0][7] = 9;
	easy_vector[1][6] = 8;
	easy_vector[1][2] = 7;
	easy_vector[1][3] = 5;
	easy_vector[1][5] = 2;
	easy_vector[2][0] = 6;
	easy_vector[2][3] = 8;
	easy_vector[2][5] = 7;
	easy_vector[2][8] = 5;
	easy_vector[3][0] = 3;
	easy_vector[3][1] = 7;
	easy_vector[3][4] = 8;
	easy_vector[3][7] = 5;
	easy_vector[3][8] = 1;
	easy_vector[4][0] = 2;
	easy_vector[4][8] = 8;
	easy_vector[5][0] = 9;
	easy_vector[5][1] = 5;
	easy_vector[5][4] = 4;
	easy_vector[5][7] = 3;
	easy_vector[5][8] = 2;
	easy_vector[6][0] = 8;
	easy_vector[6][3] = 1;
	easy_vector[6][5] = 4;
	easy_vector[6][8] = 9;
	easy_vector[7][2] = 1;
	easy_vector[7][3] = 9;
	easy_vector[7][5] = 3;
	easy_vector[7][6] = 6;
	easy_vector[8][1] = 4;
	easy_vector[8][7] = 2;

	hard_vector[0][2] = 6;
	hard_vector[1][1] = 8;
	hard_vector[1][4] = 5;
	hard_vector[1][5] = 4;
	hard_vector[1][6] = 2;
	hard_vector[2][1] = 4;
	hard_vector[2][4] = 9;
	hard_vector[2][7] = 7;
	hard_vector[3][2] = 7;
	hard_vector[3][3] = 9;
	hard_vector[3][6] = 3;
	hard_vector[4][4] = 8;
	hard_vector[4][6] = 4;
	hard_vector[5][0] = 6;
	hard_vector[5][6] = 1;
	hard_vector[6][0] = 2;
	hard_vector[6][2] = 3;
	hard_vector[6][8] = 1;
	hard_vector[7][3] = 5;
	hard_vector[7][7] = 4;
	hard_vector[8][2] = 8;
	hard_vector[8][3] = 3;
	hard_vector[8][6] = 5;
	hard_vector[8][8] = 2;

}

void SampleBoards::clear() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			clear_board[i][j] = 0;
		}
	}
}