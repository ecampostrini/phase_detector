#include <iostream>
#include <armadillo>

#include "linear_system.hpp"
#include "utils.hpp"


void LinearSystem::buildSystem()
{
	int total_non_null_indices = 0;
	int iterators_mask = getIteratorBitmask(loop_info.iterator_num);

	for(auto elem : mapping)
	{
		int current_non_null_indices = extract_index_values(elem.second, loop_info.it_bounds, coeff_matrix, num_of_equations);
		total_non_null_indices |= current_non_null_indices;
		b[num_of_equations] = elem.second;
		num_of_equations++;

		if(!(total_non_null_indices ^ iterators_mask) && num_of_equations >= loop_info.iterator_num) break;

		if(num_of_equations >= max_num_equations) break;
	}
};

bool LinearSystem::solveSystem()
{
	bool result = false;
	
	try
	{
		result = arma::solve(solution_vector, coeff_matrix, b);

		if(result) 
		{
			std::cout << "Result: ";
			for(auto coeff : solution_vector)
				std::cout << coeff << ", ";
			std::cout << std::endl;
		}
	}
	catch(...)
	{
		result = false;
	}

	return result;
}

void LinearSystem::printSystem()
{
	for(unsigned int i = 0; i < num_of_equations; i++)
	{
		for(arma::mat::row_iterator it = coeff_matrix.begin_row(i); it != coeff_matrix.end_row(i); it++)
		{
			std::cout << *it << "\t";
		}
		std::cout << "| " <<b(i) << std::endl;
	}
}

