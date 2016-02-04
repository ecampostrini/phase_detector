#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <armadillo>
#include <cassert>

#include "utils.hpp"

std::vector<int64_t> splitEntry(std::string entry_line)
{
	std::string new_value;
	std::stringstream entry_stream(entry_line);
	std::vector<int64_t> values;

	while(getline(entry_stream, new_value, ' '))
	{
		values.push_back(stol(new_value));
	}

	return values;
}

int getIteratorBitmask(int iterators_num)
{
	int result = 0x0;
	assert(iterators_num < 6 && "Trying to get bitmask for more than 5 iterators");

	switch(iterators_num)
	{
		case 1:
			result = ITERATOR_MASK(1);
			break;
		case 2:
			result = ITERATOR_MASK(2);
			break;
		case 3:
			result = ITERATOR_MASK(3);
			break;
		case 4:
			result = ITERATOR_MASK(4);
			break;
		case 5:
			result = ITERATOR_MASK(5);
			break;
		default:
			break;	
	}

	return result;
}


/** Brief it decomposes a scalar value that represents the position in a linearized array
* into the corresponding values of i, j, k...etc.
*/
int extract_index_values(int absolute_index, const std::vector<int> it_bounds, arma::Mat<double> &coeff_matrix, const unsigned int count)
{
	int bound_accumulator = 1;
	int i = 0;
	int result = 0;

	try{
		for(std::vector<int>::const_iterator it = it_bounds.begin(); it != it_bounds.end(); it++, i++)
		{
			int current_bound = *it;
			int new_index_value = (absolute_index / bound_accumulator) % current_bound;

			//this is to try to avoid null coefficients in the matrix which may lead to
			//if(new_index_value == 0) result = false;
			if(new_index_value != 0) result |= 1 << i;

			coeff_matrix(count, i) = new_index_value;
			absolute_index -= new_index_value * bound_accumulator;
			bound_accumulator *= current_bound;
		}
	}
	catch(std::logic_error e)
	{
		std::cerr << "Error filling the matrix\n";
		exit(EXIT_FAILURE);
	}

	return result;
}
