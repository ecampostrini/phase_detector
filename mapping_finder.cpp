#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include <cassert>
#include <memory>

#include "mapping_finder.hpp"
#include "utils.hpp"

void 
MappingFinder::loadMaster(std::string master_filename)
{
	std::ifstream master_file(master_filename);
	std::string new_entry;
	std::vector<int64_t> line_values;
	int addr_counter = 0;

	if(master_file)
	{
		master_map = std::make_shared<std::map<int64_t, int64_t> > ();
		std::getline(master_file, new_entry, ' ');
		loop_info.entries_num = std::stol(new_entry);
		std::getline(master_file, new_entry);
		loop_info.iterator_num = std::stol(new_entry);
		for(unsigned int i = 0; i < loop_info.iterator_num - 1; i++)
		{
			std::getline(master_file, new_entry, ' ');
			loop_info.it_bounds.insert(loop_info.it_bounds.begin(),std::stol(new_entry));
		}
		std::getline(master_file, new_entry);
		loop_info.it_bounds.insert(loop_info.it_bounds.begin(), std::stol(new_entry));

		while(std::getline(master_file, new_entry))
		{	
			line_values = splitEntry(new_entry);
			std::vector<int64_t> new_iterators(line_values.begin() + 1, line_values.end());

			iterators_vector.push_back(new_iterators);
			(*master_map)[line_values[0]] = addr_counter++; 
		}
	}
	else
	{
		std::cerr << "Couldn't open the master file\n";
		exit(EXIT_FAILURE);
	}
}

void 
MappingFinder::loadSlave(std::string slave_filename)
{
	std::ifstream slave_file(slave_filename);
	std::string new_entry;
	std::vector<int64_t> line_values;

	if(slave_file)
	{
		//we just keep the value of the address (the first one) and
		//move to the next line discarding the rest of the values
		//which belong to the iterators.
		slave_file.ignore(MAX_LINESIZE, '\n');
		slave_file.ignore(MAX_LINESIZE, '\n');
		while(std::getline(slave_file, new_entry, ' '))
		{
			slave_vector.push_back(std::stol(new_entry));
			slave_file.ignore(MAX_LINESIZE, '\n');
		}
	}
	else
	{
		std::cerr << "Couldn't open the slave file\n";
		exit(EXIT_FAILURE);
	}
}

bool MappingFinder::findMapping(std::map<int, int> &mapping)
{
	int last_position = -1;
	bool result = false;

	while(slave_position < slave_vector.size() && master_map->find(slave_vector[slave_position]) == master_map->end()) 
	{
		slave_position++;
	}

	mapping.clear();
	while(slave_position < slave_vector.size())
	{
		std::map<int64_t, int64_t>::iterator map_iterator;

		map_iterator = master_map->find(slave_vector[slave_position]);

		if((map_iterator = master_map->find(slave_vector[slave_position])) != master_map->end() && (map_iterator->second > last_position))
		{
			last_position = map_iterator->second;
			//mapping.push_back(map_iterator->second);
			assert(mapping.find(slave_position) == mapping.end() && "Slave is not linear");
			mapping[slave_position] = map_iterator->second;

			if(!result) result = true;

			slave_position++;
		}
		else
		{
			break;
		}
	}

	return result;
}
