#pragma once
#include <vector>

struct MemberOfPopulation {
    int rows = 9; // Board size 
    int cols = 9;
    int rank; // Rank is equal to 100 - number of fields left empty (with no moves left)
    // NOTE: Fields are numbered so that 1 is in the upper left corner 

    std::vector<std::vector<int>> board;
    std::vector<int> empty_fields; // Vector containing fields that need to be completed
    std::vector<int> no_more_moves; // Vector containing fields that don't have any moves possible (left empty)

    // Main constructors
    MemberOfPopulation(std::vector<std::vector<int>> in_board); // Create member of first population (no crossovers and mutations)
    MemberOfPopulation(std::vector<std::vector<int>> in_board, MemberOfPopulation& parent_1, MemberOfPopulation& parent_2); // Create member of population from reproduction

    // Additional constructors
    MemberOfPopulation();
    MemberOfPopulation(int mode);// Initialiaze first member
    MemberOfPopulation& operator=(const MemberOfPopulation& other);

    ~MemberOfPopulation();

    // General functions 
    void initialize_board_values(std::vector<std::vector<int>> in_board); // Creates input board (NO USER INPUT - hard coded)
    void set_board(std::vector<std::vector<int>> in_board); // Creates input board (from USER INPUT)
    void find_empty_fields(); // Look for empty fields and save them from 1 - first field to 81 - last field
    int read_field_value(int field);
    bool check_if_value_possible(int field, int val);
    int find_random_field(); // Select random empty field
    void find_possible_moves_and_chose_one_randomly(int field); // Select random possible value available for selected field
    void generate_solution();
    void calculate_rank();
    bool complete_solution_found(MemberOfPopulation& member);

    // Genetic functions
    void crossover(MemberOfPopulation& parent_1, MemberOfPopulation& parent_2);
    void mutation(int field, int field_2);

    // Debug functions
    void show_board();
    void show_rank();
};
