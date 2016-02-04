#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <armadillo>
#include <cassert>
#include <memory>
#include <algorithm>

#include "phase_detector.hpp"
#include "mapping_finder.hpp"
#include "linear_system.hpp"
#include "utils.hpp"

template <class T>
void print_mapping(std::map<T, T> mapping)
{
	for(auto item : mapping)
		std::cout << item.first << " -> " << item.second << "\n";
}

int main(int argc, char** argv)
{
	std::string master_filename, slave_filename;
	
	if(argc != 3)
	{
		std::cout << "usage :" << argv[0] << " master slave\n";
		exit(EXIT_FAILURE);
	}

	master_filename = argv[1];
	slave_filename = argv[2];

	///HERE WE LOOK FOR A PHASE INSIDE THE MASTER STATEMENT
	PhaseDetector phase_detector(master_filename, 100, 100);

	while(phase_detector.hasNext())
	{
		phase_detector.detectPhase();	

		if(phase_detector.hasNewPhase())
		{
			std::cout << "Phase found :)" << std::endl;
		}
	}

	///HERE WE LOOK FOR A MAPPING
	LoopInformation li;
	phase_detector.getLoopInfo(li);
	MappingFinder mapping_finder(li);

	mapping_finder.loadSlave(slave_filename);
	for(auto map_pointer : phase_detector.getPhaseVector())
	{
		std::map<int, int> new_mapping;
		mapping_finder.loadPhaseMap(map_pointer);

		while(mapping_finder.hasNext())
		{
			if(mapping_finder.findMapping(new_mapping))
			{
				assert(new_mapping.size() >= li.iterator_num && "Not enough points for finding out the linear function\n");
				std::cout << "Mapping found :)" << std::endl;
				print_mapping<int>(new_mapping);

				LinearSystem linear_system(li, new_mapping);
			
				linear_system.buildSystem();
				bool result = linear_system.solveSystem();
	
				if(!result)
					std::cout << "But system not solved :(" << std::endl;
				else
				{
					linear_system.printSystem();
					std::cout << "And system solved :)" << std::endl;
				}
			}
			else
			{
				std::cout << "No mapping found :(\n";
			}
		}
	}

	return 0;
}
