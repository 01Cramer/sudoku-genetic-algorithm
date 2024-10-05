#include "Menu.h"
#include "DemoMode.h"
#include "Solver.h"
#include "UserMode.h"
#include "LoadByImage.h"
#include "SampleBoards.h"
#include <iostream>
#include <SFML/Graphics.hpp>

sf::RenderWindow window(sf::VideoMode(800, 600), "Sudoku Solver");
const int x = window.getSize().x;
const int y = window.getSize().y;
const uint8_t menu_number_of_items = 3;
const uint8_t user_mode_number_of_items = 3;
const uint8_t load_by_image_number_of_itmes = 2;
const uint8_t demo_mode_number_of_itmes = 4;

const std::string python_script = "sudoku.py";
std::string view = "menu";
std::string encoded_sudoku_board = "";

bool thread_flag = false;
bool board_is_valid = true;

SampleBoards sample_boards;
Menu menu(x, y, menu_number_of_items);
Demo demo(x, y, demo_mode_number_of_itmes);
UserMode user_mode(x, y, user_mode_number_of_items);
LoadByImage load_image(x, y, load_by_image_number_of_itmes);

Solver solver_easy_mode(x, y, 1);
Solver solver_hard_mode(x, y, 2);
Solver solver_user_mode(x, y, 3);

std::thread solving_thread;

int main() {
	try {
		while (window.isOpen()) {
			sf::Event event;

			while (window.pollEvent(event)) {// Catching events
				switch (event.type) {
				case(sf::Event::Closed): { // Close
					window.close();
					if (thread_flag) {
						solver_easy_mode.stop();
						solver_hard_mode.stop();
						solver_user_mode.stop();
						solving_thread.join();
					}
					break;
				}

				case(sf::Event::KeyReleased): {
					if (event.key.code == sf::Keyboard::Up) { // Moving up
						if (view == "menu") {
							menu.move_up();
						}

						else if (view == "load_by_image") {
							load_image.move_up();
						}

						else if (view == "demonstration_menu") {
							bool demo_menu = true;
							demo.move_up(demo_menu);
						}

						else if (view == "demo_easy_mode") {
							bool demo_menu = false;
							demo.move_up(demo_menu);
						}

						else if (view == "demo_hard_mode") {
							bool demo_menu = false;
							demo.move_up(demo_menu);
						}
					}

					else if (event.key.code == sf::Keyboard::Down) { // Moving Down
						if (view == "menu") {
							menu.move_down();
						}

						else if (view == "load_by_image") {
							load_image.move_down();
						}

						else if (view == "demonstration_menu") {
							bool demo_menu = true;
							demo.move_down(demo_menu);
						}

						else if (view == "demo_easy_mode") {
							bool demo_menu = false;
							demo.move_down(demo_menu);
						}

						else if (view == "demo_hard_mode") {
							bool demo_menu = false;
							demo.move_down(demo_menu);
						}
					}

					else if (event.key.code == sf::Keyboard::Enter) { // Selecting 
						if (view == "menu") {
							uint8_t menu_action = menu.get_pressed_item();

							switch (menu_action) {
							case(0): {
								view = "user_mode";
								break;
							}

							case(1): {
								view = "demonstration_menu";
								break;
							}

							case(2): {
								window.close();
								break;
							}
							}
						}

						else if (view == "demonstration_menu") {
							int demo_menu_action = demo.get_pressed_item();
							switch (demo_menu_action) {
							case(1): {
								view = "demo_easy_mode";
								break;
							}

							case(2): {
								view = "demo_hard_mode";
								break;
							}

							case(3): {
								view = "menu";
								break;
							}
							}
						}

						else if (view == "load_by_image") {
							int load_by_image_action = load_image.get_pressed_item();
							switch (load_by_image_action) {
							case(0): {
								load_image.display_processing_information();
								encoded_sudoku_board = load_image.execute_python_script(python_script, load_image.path_to_image);
								load_image.clear_path_to_image();
								view = "user_mode";
								break;
								}
							case(1): {
								load_image.clear_path_to_image();
								view = "user_mode";
								break;
							}
							}
						}

						else if (view == "demo_easy_mode") {
							int demo_action = demo.get_action();
							switch (demo_action) {
							case(0): {
								solver_easy_mode.start_demo();
								solving_thread = start_calculating(solver_easy_mode, sample_boards.easy_vector);
								thread_flag = true;
								view = "solving_demo_easy_mode";
								break;
							}

							case(1): {
								view = "demonstration_menu";
								break;
							}
							}
						}

						else if (view == "solving_demo_easy_mode") {
							view = "demonstration_menu";
							solver_easy_mode.stop();
							thread_flag = false;
							solving_thread.join();
						}

						else if (view == "demo_hard_mode") {
							int demo_action = demo.get_action();
							switch (demo_action) {
							case(0): {
								solver_hard_mode.start_demo();
								solving_thread = start_calculating(solver_hard_mode, sample_boards.hard_vector);
								thread_flag = true;
								view = "solving_demo_hard_mode";
								break;
							}

							case(1): {
								view = "demonstration_menu";
								break;
							}
							}
						}

						else if (view == "solving_demo_hard_mode") {
							view = "demonstration_menu";
							solver_hard_mode.stop();
							thread_flag = false;
							solving_thread.join();
						}

						else if (view == "solving_user_mode") {
							view = "menu";
							solver_user_mode.stop();
							user_mode.reset_board();
							thread_flag = false;
							solving_thread.join();
						}
					}
				}
				case(sf::Event::TextEntered): {
					if (view == "load_by_image") {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && event.key.code == sf::Keyboard::V) {
							std::string clipboard_text = load_image.get_path_from_clipboard();
							load_image.path_to_image = clipboard_text;
							load_image.input_path.setString(load_image.path_to_image);
							load_image.input_path.setCharacterSize(14);
							sf::FloatRect inputRect = load_image.input_path.getLocalBounds();
							load_image.input_path.setPosition(sf::Vector2f(load_image.get_width() / 2, load_image.get_height() / 2));
							load_image.input_path.move(-inputRect.width / 2.0f, 0);
						}
						else if (event.key.code == sf::Keyboard::Backspace) {
							if (!load_image.path_to_image.empty()) {
								load_image.clear_path_to_image();
							}
						}
					}
					break;
				}
				}
			}

			///////// END OF EVENTS LOOP /////////

			window.clear(sf::Color(173, 173, 201));

			if (view == "menu") {
				menu.draw(window);
			}

			else if (view == "demonstration_menu") {
				demo.draw(window);
			}

			else if (view == "demo_easy_mode") {
				demo.draw_mode(window, sample_boards.easy_board);
			}

			else if (view == "demo_hard_mode") {
				demo.draw_mode(window, sample_boards.hard_board);
			}

			else if (view == "solving_demo_easy_mode") {
				solver_easy_mode.draw_solution(window);
			}

			else if (view == "solving_demo_hard_mode") {
				solver_hard_mode.draw_solution(window);
			}

			else if (view == "load_by_image") {
				load_image.draw(window);
			}

			else if (view == "user_mode") {
				sample_boards.clear();

				int response = user_mode.draw_user_mode(window, sample_boards.clear_board, encoded_sudoku_board);
				
				if (user_mode.validate_board()) {
					board_is_valid = true;
				}
				else {
					board_is_valid = false;
				}

				if (response == 0 and board_is_valid) { // Solve
					std::vector<std::vector<int>> board = user_mode.get_board();
					MemberOfPopulation member;
					member.set_board(board);
					solver_user_mode.set_best(member);
					solver_user_mode.start_user_mode();
					solving_thread = start_calculating(solver_user_mode, board);
					thread_flag = true;

					encoded_sudoku_board = "";
					view = "solving_user_mode";
				}
				else if (response == 0 and !board_is_valid) {
					user_mode.display_warning_box();
					continue;
				}
				else if (response == 1){
					view = "load_by_image";

				}
				else {
					user_mode.reset_board();
					encoded_sudoku_board = "";
					view = "menu";
				}
			}

			else if (view == "solving_user_mode") {
				solver_user_mode.draw_solution(window);
			}

			window.display(); // Display on screen
		}// END OF WINDOW LOOP
	}
	catch (std::exception error) {
		std::exit(EXIT_FAILURE);
	}
}