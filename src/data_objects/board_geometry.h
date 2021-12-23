#ifndef _BOARD_GEOMETRY_H
#define _BOARD_GEOMETRY_H

#include <vector>
#include <utility>

#include <ostream>
#include <istream>

using namespace std;

class BoardGeometry{
	unsigned int width;
	unsigned int height;
	
	vector<pair<unsigned int, unsigned int>> nodes;
	
	unsigned int city_radius;
	unsigned multi_rail_width;
public:
	BoardGeometry(
		unsigned int width, unsigned int height,
		vector<pair<unsigned int, unsigned int>> nodes,
		unsigned int city_radius, unsigned int multi_rail_width
		);
		
	unsigned int get_width();
	unsigned int get_height();
	
	const vector<pair<unsigned int, unsigned int>>& get_nodes();
	
	unsigned int get_city_radius();
	unsigned int get_multi_rail_width();
	
	void serialize(ostream& output);
	static BoardGeometry deserialize(istream& input);
};

#endif
