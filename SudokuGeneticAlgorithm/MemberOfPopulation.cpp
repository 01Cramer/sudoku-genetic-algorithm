#include "MemberOfPopulation.h"
#include "SampleBoards.h"
#include <iostream>
#include <random>
#include<algorithm>


/// Constructors ///

MemberOfPopulation::MemberOfPopulation(std::vector<std::vector<int>> in_board) : board(rows, std::vector<int>(cols)) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = 0;
        }
    }

    initialize_board_values(in_board);
    find_empty_fields();
    generate_solution();
    calculate_rank();
}
MemberOfPopulation::MemberOfPopulation(std::vector<std::vector<int>> in_board, MemberOfPopulation& parent_1, MemberOfPopulation& parent_2) : board(rows, std::vector<int>(cols)) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = 0;
        }
    }

    initialize_board_values(in_board);
    find_empty_fields();
    crossover(parent_1, parent_2);
    calculate_rank();
}

MemberOfPopulation::MemberOfPopulation() : board(rows, std::vector<int>(cols)) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = 0;
        }
    }
}

MemberOfPopulation::MemberOfPopulation(int mode) : board(rows, std::vector<int>(cols)) {
    SampleBoards sample_boards;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = 0;
        }
    }

    if (mode == 1) { // An example of an easy board - for demonstration
        board = sample_boards.easy_vector;
    }

    else if (mode == 2) { // An example of an hard board - for demonstration
        board = sample_boards.hard_vector;
    }
}

MemberOfPopulation :: ~MemberOfPopulation() {

}

/// Functions ///

void MemberOfPopulation::initialize_board_values(std::vector<std::vector<int>> in_board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = in_board[i][j];
        }
    }
}

void MemberOfPopulation::find_empty_fields() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == 0) {
                int field_number = 9 * i + j + 1;
                empty_fields.push_back(field_number);
            }
        }
    }
}

int MemberOfPopulation::find_random_field() {
    std::random_device device;
    std::uniform_int_distribution<int> rand_field(0, empty_fields.size() - 1); // From 0 because index from vector 
    int random_field = empty_fields[rand_field(device)];

    return random_field;
}

void MemberOfPopulation::find_possible_moves_and_chose_one_randomly(int field) {
    std::vector<int> possible_moves = { 1,2,3,4,5,6,7,8,9 };
    int row = (field - 1) / 9;
    int col = (field - 1) % 9;

    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;

    for (int i = 0; i < cols; i++) { // Search row
        if (board[row][i] != 0) {
            if (find(possible_moves.begin(), possible_moves.end(), board[row][i]) != possible_moves.end()) {
                possible_moves.erase(std::remove(possible_moves.begin(), possible_moves.end(), board[row][i]), possible_moves.end());
            }
        }
    }

    for (int i = 0; i < rows; i++) { // Search column
        if (board[i][col] != 0) {
            if (find(possible_moves.begin(), possible_moves.end(), board[i][col]) != possible_moves.end()) {
                possible_moves.erase(std::remove(possible_moves.begin(), possible_moves.end(), board[i][col]), possible_moves.end());
            }
        }
    }

    // Search square 3x3
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (find(possible_moves.begin(), possible_moves.end(), board[i][j]) != possible_moves.end()) {
                possible_moves.erase(std::remove(possible_moves.begin(), possible_moves.end(), board[i][j]), possible_moves.end());
            }
        }
    }

    if (possible_moves.size() == 0) { // No more moves for this field 
        no_more_moves.emplace_back(field);
        empty_fields.erase(std::remove(empty_fields.begin(), empty_fields.end(), field), empty_fields.end());
    }

    else { // Make random move from possible moves
        empty_fields.erase(std::remove(empty_fields.begin(), empty_fields.end(), field), empty_fields.end());
        std::random_device device;
        std::uniform_int_distribution<int> rand_val(0, possible_moves.size() - 1);
        board[row][col] = possible_moves[rand_val(device)];
    }
}

void MemberOfPopulation::generate_solution() { // Try to complete board until there is no possible move left
    while (empty_fields.size() != 0) {
        int field = find_random_field();
        find_possible_moves_and_chose_one_randomly(field);
    }
}

void MemberOfPopulation::calculate_rank() {
    rank = (no_more_moves.size() == 0) ? 100 : 100 - no_more_moves.size();
}

bool MemberOfPopulation::check_if_value_possible(int field, int val) { // For crossovers
    int row = (field - 1) / 9;
    int col = (field - 1) % 9;
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;

    for (int i = 0; i < cols; i++) { // Search row
        if (board[row][i] == val) {
            return false;
        }
    }

    for (int i = 0; i < rows; i++) { // Search column
        if (board[i][col] == val) {
            return false;
        }
    }

    // Search square 3x3
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (board[i][j] == val) {
                return false;
            }
        }
    }

    return true;
}

int MemberOfPopulation::read_field_value(int field) {
    int row = (field - 1) / 9;
    int col = (field - 1) % 9;

    return board[row][col];
}

bool MemberOfPopulation::complete_solution_found(MemberOfPopulation& member) {
    if (member.no_more_moves.size() == 0) {
        return true;
    }

    return false;
}

void MemberOfPopulation::set_board(std::vector<std::vector<int>> in_board) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = in_board[i][j];
        }
    }
}

MemberOfPopulation& MemberOfPopulation :: operator=(const MemberOfPopulation& other) {
    rows = other.rows;
    cols = other.cols;
    empty_fields = other.empty_fields;
    board = other.board;
    no_more_moves = other.no_more_moves;
    rank = other.rank;

    return *this;
}

/// Debug functions ///

void MemberOfPopulation::show_board() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << board[i][j] << " ";
            if (j == 2 or j == 5) {
                std::cout << "|";
            }

            if (j == 8) {
                std::cout << std::endl;
            }
        }

        if (i == 2 or i == 5) {
            for (int x = 0; x < cols * 2; x++) {
                std::cout << "-";
            }
            std::cout << std::endl;
        }
    }
}
void MemberOfPopulation::show_rank() {
    std::cout << rank << std::endl;
}