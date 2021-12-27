#ifndef _PLAYER_INTERFACE_H
#define _PLAYER_INTERFACE_H

#include <vector>

using namespace std;

class PlayerInterface{
public:
	virtual bool add_station(unsigned int node_index) = 0;
	virtual bool play(vector<unsigned int> edge_indices) = 0;
};

#endif
