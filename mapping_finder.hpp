#ifndef MAPPING_H
#define MAPPING_H

#include <string>
#include <map>
#include <iostream>
#include <memory>

#include "phase_detector.hpp"

class MappingFinder
{
	public:
	
	MappingFinder(LoopInformation li) : master_map(NULL), slave_position(0), loop_info(li) {};
	~MappingFinder(){ };

	void loadMaster(std::string master_filename);

	void loadPhaseMap(std::shared_ptr<std::map<int64_t, int64_t> > pm) 
	{
		slave_position = 0;
		master_map = pm;
	};

	void loadSlave(std::string slave_filename);

	bool findMapping(std::map<int, int> &mapping);
	bool hasNext() { return slave_position < slave_vector.size(); };

	private:

	std::shared_ptr<std::map<int64_t, int64_t> > master_map;
	std::vector<std::vector<int64_t> > iterators_vector;
	std::vector<int64_t> slave_vector;
	unsigned int slave_position;
	LoopInformation loop_info;
};

#endif
