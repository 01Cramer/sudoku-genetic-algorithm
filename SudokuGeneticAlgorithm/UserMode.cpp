#include "UserMode.h"

UserMode::UserMode(float width, float height) : board_to_solve(rows, std::vector<int>(cols)) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board_to_solve[i][j] = 0;
        }
    }

    if (!font.loadFromFile("Fonts/Alice-Regular.ttf")) {
        throw std::exception("Cant find font!");
    }

    sf::FloatRect textRect;
    user_mode[0].setFont(font);
    user_mode[0].setCharacterSize(64);
    user_mode[0].setFillColor(sf::Color(111, 0, 255));
    user_mode[0].setString("Start Solving");
    user_mode[0].setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 250.0f));
    textRect = user_mode[0].getLocalBounds();
    user_mode[0].setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);

    user_mode[1].setFont(font);
    user_mode[1].setCharacterSize(64);
    user_mode[1].setFillColor(sf::Color::Black);
    user_mode[1].setString("Back");
    user_mode[1].setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 180.0f));
    textRect = user_mode[1].getLocalBounds();
    user_mode[1].setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    selected_item_index = 0;
}

UserMode :: ~UserMode() {

}

int UserMode::draw_user_mode(sf::RenderWindow& window, int sudoku_board[9][9]) {
    sf::Font font;

    if (!font.loadFromFile("Fonts/arial.ttf")) {
        throw std::exception("Can't find font!");
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

    sf::Event event;
    sf::Vector2i selectedCell(-1, -1);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    if (mousePosition.x >= boardLeft && mousePosition.x < boardLeft + boardWidth &&
                        mousePosition.y >= boardTop && mousePosition.y < boardTop + boardHeight) { // Check if mouse was clicked on the board
                        // Count index
                        int columnIndex = (mousePosition.x - boardLeft) / cellSize;
                        int rowIndex = (mousePosition.y - boardTop) / cellSize;
                        selectedCell.x = columnIndex;
                        selectedCell.y = rowIndex;
                    }
                }
            }

            else if (event.type == sf::Event::TextEntered) {
                if (selectedCell.x != -1 && selectedCell.y != -1) {
                    if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                        int value = event.text.unicode - '0';
                        int row = selectedCell.y;
                        int col = selectedCell.x;
                        bool possible = true;

                        for (int i = 0; i < 9; i++) {// Search row
                            if (sudoku_board[selectedCell.y][i] == value and value != 0) {
                                possible = false;
                            }
                        }

                        for (int i = 0; i < 9; i++) {// Search column
                            if (sudoku_board[i][selectedCell.x] == value and value != 0) {
                                possible = false;
                            }
                        }

                        int startRow = (row / 3) * 3;
                        int startCol = (col / 3) * 3;

                        for (int i = startRow; i < startRow + 3; i++) { // Search 3x3 square
                            for (int j = startCol; j < startCol + 3; j++) {
                                if (sudoku_board[i][j] == value and value != 0) {
                                    possible = false;
                                }
                            }
                        }

                        if (possible) {
                            sudoku_board[selectedCell.y][selectedCell.x] = value;
                            board_to_solve[selectedCell.y][selectedCell.x] = value;
                        }
                    }
                }
            }

            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) {
                    move_up();
                }

                else if (event.key.code == sf::Keyboard::Down) {
                    move_down();
                }

                else if (event.key.code == sf::Keyboard::Enter) {
                    return selected_item_index;
                }
            }
        }

        window.clear(sf::Color(173, 173, 201));

        window.draw(board);

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

        if (selectedCell.x != -1 && selectedCell.y != -1) {
            sf::RectangleShape selectedCellHighlight(sf::Vector2f(cellSize - 2, cellSize - 2));
            selectedCellHighlight.setPosition(boardLeft + selectedCell.x * cellSize + 1, boardTop + selectedCell.y * cellSize + 1);
            selectedCellHighlight.setFillColor(sf::Color(255, 255, 0, 128));
            window.draw(selectedCellHighlight);
        }

        for (int i = 0; i < 2; ++i) {
            window.draw(user_mode[i]);
        }

        window.display();
    }
}


void UserMode::move_up() {
    if (selected_item_index - 1 >= 0) {
        user_mode[selected_item_index].setFillColor(sf::Color::Black);
        selected_item_index--;
        user_mode[selected_item_index].setFillColor(sf::Color(111, 0, 255));
    }
}

void UserMode::move_down() {
    if (selected_item_index + 1 <= 1) {
        user_mode[selected_item_index].setFillColor(sf::Color::Black);
        selected_item_index++;
        user_mode[selected_item_index].setFillColor(sf::Color(111, 0, 255));
    }
}

int UserMode::get_pressed_item() {
    return selected_item_index;
}

std::vector<std::vector<int>> UserMode::get_board() {
    return board_to_solve;
}

void UserMode::reset_board() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board_to_solve[i][j] = 0;
        }
    }
}

UserMode::UserMode() : board_to_solve(rows, std::vector<int>(cols)) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board_to_solve[i][j] = 0;
        }
    }
}