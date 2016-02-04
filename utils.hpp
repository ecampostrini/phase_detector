#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <armadillo>

#define MAX_LINESIZE std::numeric_limits<std::streamsize>::max()

#define BITMASK_1 0x1
#define BITMASK_2 0x3
#define BITMASK_3 0x7
#define BITMASK_4 0xF
#define BITMASK_5 0x1F
#define ITERATOR_MASK(n) BITMASK_##n


std::vector<int64_t> splitEntry(std::string);
int getIteratorBitmask(int n);
int extract_index_values(int absolute_index, const std::vector<int> it_bounds, arma::Mat<double> &coeff_matrix, const unsigned int count);

template<class T>
void print_system(arma::Mat<T> matrix_coeff, arma::Col<T> b, unsigned int size)
{
	for(unsigned int i = 0; i < size; i++)
	{
		for(arma::mat::row_iterator it = matrix_coeff.begin_row(i); it != matrix_coeff.end_row(i); it++)
		{
			std::cout << *it << "\t";
		}
		std::cout << "| " <<b(i) << std::endl;
	}
}

#endif
