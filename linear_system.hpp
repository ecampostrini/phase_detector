#ifndef LINEAR_SYSTEM_H
#define LINEAR_SYSTEM_H

#include <armadillo>
#include <map>
#include <iostream>

#include "phase_detector.hpp"

class LinearSystem
{
	public:
	
	LinearSystem(LoopInformation &li, std::map<int, int> &m) : 
	loop_info(li),
	mapping(m),
	num_unknowns(loop_info.iterator_num),
	num_of_equations(0)
	{
		max_num_equations = li.iterator_num + 4;
		coeff_matrix = arma::Mat<double>(max_num_equations, num_unknowns);
		b = arma::Col<double>(max_num_equations);
	};
	~LinearSystem(){};

	void buildSystem();
	bool solveSystem();
	arma::vec getSolution(){ return solution_vector;};
	void printSystem();

	private:

	unsigned int max_num_equations;
	LoopInformation &loop_info;
	std::map<int, int> mapping;
	unsigned int num_unknowns;
	unsigned int num_of_equations; 

	arma::Mat<double> coeff_matrix;
	arma::Col<double> b; 
	arma::vec solution_vector;
};

#endif


