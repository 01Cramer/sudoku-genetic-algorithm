#include "Menu.h"
#include "DemoMode.h"
#include "Solver.h"
#include "UserMode.h"
#include "SampleBoards.h"
#include <SFML/Graphics.hpp>
#include<thread>


std::thread start_thread(Solver& solver, std::vector<std::vector<int>> in_board) {
	std::thread solve(&Solver::solve, &solver, in_board);

	return solve;
}


sf::RenderWindow window(sf::VideoMode(800, 600), "Sudoku Solver");
const int x = window.getSize().x;
const int y = window.getSize().y;

std::thread solving_thread;

int main() {
	try {
		SampleBoards sample_boards;

		Menu menu(x, y);
		Demo demo(x, y);
		UserMode user_mode(x, y);

		Solver solver_easy_mode(x, y, 1);
		Solver solver_hard_mode(x, y, 2);
		Solver solver_user_mode(x, y, 3);
		bool thread_flag = false;

		std::string view = "menu";

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
							int menu_action = menu.get_pressed_item();

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

						else if (view == "demo_easy_mode") {
							int demo_action = demo.get_action();
							switch (demo_action) {
							case(0): {
								solver_easy_mode.start_demo();
								solving_thread = start_thread(solver_easy_mode, sample_boards.easy_vector);
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
								solving_thread = start_thread(solver_hard_mode, sample_boards.hard_vector);
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

			else if (view == "user_mode") {
				sample_boards.clear();

				int response = user_mode.draw_user_mode(window, sample_boards.clear_board);

				if (response == 0) { // Solve
					std::vector<std::vector<int>> board = user_mode.get_board();
					MemberOfPopulation member;
					member.set_board(board);
					solver_user_mode.set_best(member);
					solver_user_mode.start_user_mode();
					solving_thread = start_thread(solver_user_mode, board);
					thread_flag = true;

					view = "solving_user_mode";
				}

				else { // Back
					user_mode.reset_board();
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