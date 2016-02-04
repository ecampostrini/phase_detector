#ifndef PHASES_H
#define PHASES_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>

using namespace std;

typedef struct LI
{

	unsigned int iterator_num;
	unsigned int entries_num;
	std::vector<int> it_bounds;

} LoopInformation;

class PhaseDetector
{
	public:

	PhaseDetector(std::string input_filename, unsigned int max_phase_size, unsigned int elems_to_read);
	~PhaseDetector() {};

	void detectPhase();
	bool hasNext();
	void getLoopInfo(LoopInformation &li);
	bool hasNewPhase() { return has_new_phase;}
	std::vector<std::shared_ptr<std::map<int64_t, int64_t> > > getPhaseVector() { return phase_vector; }

	//friend class MappingFinder;

	private:
	std::vector<std::shared_ptr<std::map<int64_t, int64_t> > > phase_vector;
	unsigned int iterator_num, entries_num;
	std::vector<int> bounds;
	unsigned int max_phase_size, max_elems;
	std::vector<int64_t> addr_vector;
	std::vector< std::vector<int64_t> > iterators_vector;
	unsigned int next_start;
	bool has_new_phase;

};
#endif
