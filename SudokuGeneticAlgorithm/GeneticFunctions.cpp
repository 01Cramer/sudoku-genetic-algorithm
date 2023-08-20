#include "MemberOfPopulation.h"
#include <random>


void MemberOfPopulation::mutation(int field, int field_2) { // Mutation is just taking random value not considering parents genes 
	find_possible_moves_and_chose_one_randomly(field);
	if (field != field_2) { // In case if only one field left - in other cases mutation happens on 2 fields
		find_possible_moves_and_chose_one_randomly(field_2);
	}
}

void MemberOfPopulation::crossover(MemberOfPopulation& parent_1, MemberOfPopulation& parent_2) {
	while (empty_fields.size() != 0) {
		int field = MemberOfPopulation::find_random_field();
		int field_in_case_of_mutation = MemberOfPopulation::find_random_field();

		while (field == field_in_case_of_mutation and empty_fields.size() > 1) {
			field_in_case_of_mutation = MemberOfPopulation::find_random_field();
		}

		std::random_device device;
		std::uniform_int_distribution<int> mutation_prob(1, 100);
		std::uniform_int_distribution<int> parent(1, 2);
		int generated_number = mutation_prob(device);

		if (generated_number <= 3) { // Chance of mutation is 3% 
			mutation(field, field_in_case_of_mutation);
		}

		else {
			int parent_number = parent(device);
			if (parent_number == 1) { // Random parent choice 
				int value = parent_1.read_field_value(field);
				if (check_if_value_possible(field, value)) {
					int row = (field - 1) / 9;
					int col = (field - 1) % 9;
					empty_fields.erase(std::remove(empty_fields.begin(), empty_fields.end(), field), empty_fields.end());
					board[row][col] = value;
				}

				else { // Cant put value from parent
					no_more_moves.emplace_back(field);
					empty_fields.erase(std::remove(empty_fields.begin(), empty_fields.end(), field), empty_fields.end());
				}
			}

			else {
				int value = parent_2.read_field_value(field);
				if (check_if_value_possible(field, value)) {
					int row = (field - 1) / 9;
					int col = (field - 1) % 9;
					empty_fields.erase(std::remove(empty_fields.begin(), empty_fields.end(), field), empty_fields.end());
					board[row][col] = value;
				}

				else { // Cant put value from parent 
					no_more_moves.emplace_back(field);
					empty_fields.erase(std::remove(empty_fields.begin(), empty_fields.end(), field), empty_fields.end());
				}
			}
		}

	}

}