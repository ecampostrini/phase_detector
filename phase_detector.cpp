#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <memory>

#include "phase_detector.hpp"
#include "utils.hpp"

PhaseDetector::PhaseDetector(std::string input_filename, unsigned int max_phase_sz, unsigned int elems_to_read) : 
	max_phase_size(max_phase_sz), 
	max_elems(elems_to_read),
	has_new_phase(false)
{
	std::ifstream input_file(input_filename);
	std::vector<int64_t> line_values;
	std::string input_str;

	if(input_file)
	{
		///reading the number of entries and the number of iterators
		std::getline(input_file, input_str, ' ');
		entries_num = std::stol(input_str);
		std::getline(input_file, input_str);
		iterator_num = std::stol(input_str);

		///reading the iterators' bounds	
		for(unsigned int i = 0; i < iterator_num - 1; i++)
		{
			std::getline(input_file, input_str, ' ');
			bounds.insert(bounds.begin(),std::stol(input_str));
		}
		std::getline(input_file, input_str);
		bounds.insert(bounds.begin(), std::stol(input_str));
		
		///reading the memory positions and their corresponding iteration vectors
		for(unsigned int i = 0; i < max_elems; i++)
		{
			std::getline(input_file, input_str,'\n');
			if(input_file.eof()) break;

			line_values = splitEntry(input_str);

			std::vector<int64_t> iterators(line_values.begin() + 1, line_values.end());
			addr_vector.push_back(line_values[0]);
			iterators_vector.push_back(iterators);
		}
		next_start = 0;
	}
	else
	{
		cerr << "Couldn't open the input file\n";
		exit(EXIT_FAILURE);
	}
	input_file.close();	
}

void 
PhaseDetector::detectPhase()
{
	unsigned int i, offset;
	std::unordered_map<int64_t, void*> address_hash;
	std::shared_ptr<std::map<int64_t, int64_t> > new_phase_map(new std::map<int64_t, int64_t>);

	has_new_phase = false;
	for(i = 0, offset = next_start; offset < addr_vector.size() && i < max_phase_size; i++, offset = i + next_start)
	{
		int64_t addr = addr_vector[offset];
		if(address_hash.find(addr) == address_hash.end())
		{
			address_hash[addr] = NULL;
			//(*new_phase_map)[addr] = offset;
			(*new_phase_map)[offset] = addr;
			has_new_phase = true;
		}
		else
			break;
	}

	if(has_new_phase) phase_vector.push_back(new_phase_map);
	next_start += i;
}

bool 
PhaseDetector::hasNext(){ return next_start < addr_vector.size(); }

void
PhaseDetector::getLoopInfo(LoopInformation &li)
{
	li.iterator_num = iterator_num;
	li.entries_num = entries_num;
	li.it_bounds = bounds;
}
