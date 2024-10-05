#pragma once
#include <SFML/Graphics.hpp>
#include "MemberOfPopulation.h"
#include <atomic>
#include <thread>

class Solver {
public:
		Solver(float width, float height, int mode);
		~Solver();
		void solve(std::vector<std::vector<int>> inboard); // Main evolution loop (evolution algorithm)
		void set_best(MemberOfPopulation best);
		void draw_solution(sf::RenderWindow& window);
		void draw(sf::RenderWindow& window);
		void stop(); // Stops "solve" function

		void start_demo(); /// Resets atomic variable for new problems 
		void start_user_mode();

		MemberOfPopulation best_member;
		MemberOfPopulation init_best;

	private:
		std::atomic<bool> should_stop;
		sf::Font font;
		sf::Font back_font;
		sf::Text solver_back[1];
};

// Computational thread
std::thread start_calculating(Solver& solver, std::vector<std::vector<int>> in_board);