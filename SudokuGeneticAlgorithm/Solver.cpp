#include "Solver.h"
#include <algorithm>
#include <random>



Solver::Solver(float width, float height, int mode) {
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        throw std::exception("Cant find font!");
    }

    if (!back_font.loadFromFile("Fonts/Alice-Regular.ttf")) {
        throw std::exception("Cant find font!");
    }

    should_stop = false;
    MemberOfPopulation first(mode);
    init_best = first;
    set_best(init_best);
    solver_back[0].setFont(back_font);
    solver_back[0].setCharacterSize(78);
    solver_back[0].setFillColor(sf::Color(111, 0, 255));
    solver_back[0].setString("Back");
    solver_back[0].setPosition(sf::Vector2f(width / 2, height / 2.0f - 250.0f));
    sf::FloatRect textRect = solver_back[0].getLocalBounds();
    solver_back[0].move(-textRect.width / 2.0f, 0);
}

Solver :: ~Solver() {}

void Solver::solve(std::vector<std::vector<int>> inboard) {
    std::vector<MemberOfPopulation> population;
    const int POPULATION_SIZE = 1000;

    // Generate first generation
    for (int i = 0; i < POPULATION_SIZE; i++) {
        MemberOfPopulation MemberOfPopulation(inboard);
        population.push_back(MemberOfPopulation);
    }

    std::vector<MemberOfPopulation> best_out_of_population;
    const int BEST_FROM_POPULATION_SIZE = 300;
    int generation = 1;
    int restarts = 0;

    do {
        if (should_stop) {
            break;
        }

        if (generation == 250) { // Reset solution 
            restarts++;
            population.clear();
            for (int i = 0; i < POPULATION_SIZE; i++) { // Generate first generation
                MemberOfPopulation MemberOfPopulation(inboard);
                population.push_back(MemberOfPopulation);
            }
            generation = 1;
        }

        best_out_of_population.clear();
        std::sort(population.begin(), population.end(), [](const MemberOfPopulation& lhs, const MemberOfPopulation& rhs) {return lhs.rank > rhs.rank; });

        for (int i = 0; i < BEST_FROM_POPULATION_SIZE; i++) { // Select best from first population. Method : Rank Selection
            best_out_of_population.push_back(population[i]);
        }

        population.clear();

        for (int i = 0; i < POPULATION_SIZE; i++) { // Reproduction of best members
            std::random_device device;
            std::uniform_int_distribution<int> parent(0, BEST_FROM_POPULATION_SIZE - 1);
            int parent_1 = parent(device);
            int parent_2 = parent(device);
            if (parent_1 == parent_2) { // Make sure we dont crossover on a single parent
                parent_2 = parent(device);
            }

            MemberOfPopulation member(inboard, best_out_of_population[parent_1], best_out_of_population[parent_2]);
            population.push_back(member);
        }

        generation++; // Another generation is created
        set_best(best_out_of_population[0]);

    } while (best_out_of_population[0].rank != 100);
}

void Solver::draw_solution(sf::RenderWindow& window) {
    window.clear(sf::Color(173, 173, 201));
    window.draw(solver_back[0]);
    sf::Text text("", font, 20);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);

    const int cellSize = 35;
    const int boardWidth = 9 * cellSize;
    const int boardHeight = 9 * cellSize;

    int boardLeft = (window.getSize().x - boardWidth) / 2;
    int boardTop = (window.getSize().y - boardHeight) / 1.5;

    sf::RectangleShape board(sf::Vector2f(boardWidth, boardHeight));
    board.setPosition(boardLeft, boardTop);
    board.setOutlineThickness(5);
    board.setOutlineColor(sf::Color::Black);
    board.setFillColor(sf::Color::White);
    window.draw(board);

    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color::Black);
    cell.setFillColor(sf::Color::White);

    sf::Text xText("X", font, 20);
    xText.setFillColor(sf::Color::Red);

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int left = boardLeft + j * cellSize;
            int top = boardTop + i * cellSize;
            cell.setPosition(left, top);
            window.draw(cell);

            int value = best_member.board[i][j];

            if (value != 0) {
                text.setString(std::to_string(value));
                text.setPosition(left + 10, top + 5);
                window.draw(text);
            }

            else {
                xText.setPosition(left + 10, top + 5);
                window.draw(xText);
            }
        }
    }

    for (int i = 1; i <= 8; ++i) {
        if (i % 3 == 0) {
            sf::RectangleShape horizontalLine(sf::Vector2f(boardWidth, 3));
            horizontalLine.setPosition(boardLeft, boardTop + i * cellSize);
            horizontalLine.setFillColor(sf::Color::Black);
            window.draw(horizontalLine);
        }

        else {
            sf::RectangleShape horizontalLine(sf::Vector2f(boardWidth, 1));
            horizontalLine.setPosition(boardLeft, boardTop + i * cellSize);
            horizontalLine.setFillColor(sf::Color::Black);
            window.draw(horizontalLine);
        }

        if (i % 3 == 0) {
            sf::RectangleShape verticalLine(sf::Vector2f(3, boardHeight));
            verticalLine.setPosition(boardLeft + i * cellSize, boardTop);
            verticalLine.setFillColor(sf::Color::Black);
            window.draw(verticalLine);
        }

        else {
            sf::RectangleShape verticalLine(sf::Vector2f(1, boardHeight));
            verticalLine.setPosition(boardLeft + i * cellSize, boardTop);
            verticalLine.setFillColor(sf::Color::Black);
            window.draw(verticalLine);
        }
    }
}

void Solver::draw(sf::RenderWindow& window) {
    window.draw(solver_back[0]);
}


void Solver::set_best(MemberOfPopulation member) {
    best_member = member;

}

void Solver::stop() {
    should_stop = true;
}

void Solver::start_demo() {
    should_stop = false;

    best_member = init_best;
}

void Solver::start_user_mode() {

    should_stop = false;

}