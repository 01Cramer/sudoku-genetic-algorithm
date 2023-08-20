#include "DemoMode.h"

Demo::Demo(float width, float height) {

    if (!font.loadFromFile("Fonts/Alice-Regular.ttf")) {
        throw std::exception("Cant find font!");
    }

    demo[0].setFont(font);
    demo[0].setCharacterSize(78);
    demo[0].setFillColor(sf::Color::Black);
    demo[0].setString("Mode:");
    demo[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS_DEMO + 1) * 1 - 70));
    sf::FloatRect textRect = demo[0].getLocalBounds();
    demo[0].move(-textRect.width / 2.0f, 0);

    demo[1].setFont(font);
    demo[1].setCharacterSize(64);
    demo[1].setFillColor(sf::Color(111, 0, 255));
    demo[1].setString("Easy");
    demo[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS_DEMO + 1) * 2 - 70));
    textRect = demo[1].getLocalBounds();
    demo[1].move(-textRect.width / 2.0f, 0);

    demo[2].setFont(font);
    demo[2].setCharacterSize(64);
    demo[2].setFillColor(sf::Color::Black);
    demo[2].setString("Hard");
    demo[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS_DEMO + 1) * 3 - 70));
    textRect = demo[2].getLocalBounds();
    demo[2].move(-textRect.width / 2.0f, 0);

    demo[3].setFont(font);
    demo[3].setCharacterSize(64);
    demo[3].setFillColor(sf::Color::Black);
    demo[3].setString("Back To Main Menu");
    demo[3].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS_DEMO + 1) * 4 - 70));
    textRect = demo[3].getLocalBounds();
    demo[3].move(-textRect.width / 2.0f, 0);

    solve_or_back[0].setFont(font);
    solve_or_back[0].setCharacterSize(64);
    solve_or_back[0].setFillColor(sf::Color(111, 0, 255));
    solve_or_back[0].setString("Start Solving");
    solve_or_back[0].setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 250.0f));
    textRect = solve_or_back[0].getLocalBounds();
    solve_or_back[0].setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);

    solve_or_back[1].setFont(font);
    solve_or_back[1].setCharacterSize(64);
    solve_or_back[1].setFillColor(sf::Color::Black);
    solve_or_back[1].setString("Back");
    solve_or_back[1].setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 180.0f));
    textRect = solve_or_back[1].getLocalBounds();
    solve_or_back[1].setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);

    selected_item_index = 1;
    action = 0;
}

Demo :: ~Demo() {}

void Demo::draw(sf::RenderWindow& window) {
    for (int i = 0; i < MAX_NUMBER_OF_ITEMS_DEMO; i++) {
        window.draw(demo[i]);
    }
}

void Demo::draw_mode(sf::RenderWindow& window, int sudoku_board[9][9]) {
    sf::Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        throw std::exception("Cant find font!");
    }

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

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int left = boardLeft + j * cellSize;
            int top = boardTop + i * cellSize;
            cell.setPosition(left, top);
            window.draw(cell);

            int value = sudoku_board[i][j];

            if (value != 0) {
                text.setString(std::to_string(value));
                text.setPosition(left + 10, top + 5);
                window.draw(text);
            }
        }
    }
    // Make 3x3 squares thicker
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
    // Draw text 
    for (int i = 0; i < 2; ++i) {
        window.draw(solve_or_back[i]);
    }
}

void Demo::move_up(bool modes) {
    if (modes) {
        if (selected_item_index - 1 >= 1) {
            demo[selected_item_index].setFillColor(sf::Color::Black);
            selected_item_index--;
            demo[selected_item_index].setFillColor(sf::Color(111, 0, 255));
        }
    }

    else {
        if (action - 1 >= 0) {
            solve_or_back[action].setFillColor(sf::Color::Black);
            action--;
            solve_or_back[action].setFillColor(sf::Color(111, 0, 255));
        }
    }
}

void Demo::move_down(bool modes) {
    if (modes) {
        if (selected_item_index + 1 <= 3) {
            demo[selected_item_index].setFillColor(sf::Color::Black);
            selected_item_index++;
            demo[selected_item_index].setFillColor(sf::Color(111, 0, 255));
        }
    }

    else {
        if (action + 1 <= 1) {
            solve_or_back[action].setFillColor(sf::Color::Black);
            action++;
            solve_or_back[action].setFillColor(sf::Color(111, 0, 255));
        }
    }
}

int Demo::get_pressed_item() {
    return selected_item_index;
}

int Demo::get_action() {
    return action;
}