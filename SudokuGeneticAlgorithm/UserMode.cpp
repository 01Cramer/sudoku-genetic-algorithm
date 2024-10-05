#include "UserMode.h"
#include <iostream>
#include <set>

UserMode::UserMode(float width, float height, uint8_t number_of_items) : board_to_solve(rows, std::vector<int>(cols)), Controller(number_of_items) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board_to_solve[i][j] = 0;
        }
    }

    if (!font.loadFromFile("Fonts/Alice-Regular.ttf")) {
        throw std::exception("Cant find font!");
    }

    sf::FloatRect textRect;
    menu[0].setFont(font);
    menu[0].setCharacterSize(32);
    menu[0].setFillColor(sf::Color(111, 0, 255));
    menu[0].setString("Start Solving");
    menu[0].setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 250.0f));
    textRect = menu[0].getLocalBounds();
    menu[0].setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);

    
    menu[1].setFont(font);
    menu[1].setCharacterSize(32);
    menu[1].setFillColor(sf::Color::Black);
    menu[1].setString("Load Board By Image");
    menu[1].setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 210.0f));
    textRect = menu[1].getLocalBounds();
    menu[1].setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    
    menu[2].setFont(font);
    menu[2].setCharacterSize(32);
    menu[2].setFillColor(sf::Color::Black);
    menu[2].setString("Back");
    menu[2].setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 170.0f));
    textRect = menu[2].getLocalBounds();
    menu[2].setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);

    selected_item_index = 0;
}

UserMode :: ~UserMode() {

}

int UserMode::draw_user_mode(sf::RenderWindow& window, int sudoku_board[9][9], std::string board_encoded) {
    sf::Font font;

    if (!font.loadFromFile("Fonts/arial.ttf")) {
        throw std::exception("Can't find font!");
    }
    
    if (board_encoded.size() == 81) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                sudoku_board[i][j] = board_encoded[i * 9 + j] - '0';
                board_to_solve[i][j] = board_encoded[i * 9 + j] - '0';
            }
        }
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

        for (int i = 0; i < 3; ++i) {
            window.draw(menu[i]);
        }

        window.display();
    }
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

UserMode::UserMode() : board_to_solve(rows, std::vector<int>(cols)), Controller(){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board_to_solve[i][j] = 0;
        }
    }
}

bool isValidGroup(const std::vector<int>& group) {
    std::set<int> seen;
    for (int num : group) {
        if (num != 0) {
            if (seen.find(num) != seen.end()) {
                return false; // Duplicated number found
            }
            seen.insert(num);
        }
    }
    return true;
}

bool UserMode::validate_board() {
    // Check rows
    for (int i = 0; i < 9; ++i) {
        if (!isValidGroup(board_to_solve[i])) {
            return false;
        }
    }

    // Check columns
    for (int j = 0; j < 9; ++j) {
        std::vector<int> column;
        for (int i = 0; i < 9; ++i) {
            column.push_back(board_to_solve[i][j]);
        }
        if (!isValidGroup(column)) {
            return false;
        }
    }

    // Check 3x3 sub-grids
    for (int boxRow = 0; boxRow < 3; ++boxRow) {
        for (int boxCol = 0; boxCol < 3; ++boxCol) {
            std::vector<int> block;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    block.push_back(board_to_solve[boxRow * 3 + i][boxCol * 3 + j]);
                }
            }
            if (!isValidGroup(block)) {
                return false;
            }
        }
    }
    return true;
}

int UserMode::display_warning_box() {
    int msgboxID = MessageBox(
        NULL,
        L"Loaded board is not valid\nPlease make sure board is valid.",
        L"Board is not valid",
        MB_ICONWARNING | MB_OK
    );

    return msgboxID;
}