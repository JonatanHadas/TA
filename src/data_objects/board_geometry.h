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
		
	unsigned int get_width() const;
	unsigned int get_height() const;
	
	const vector<pair<unsigned int, unsigned int>>& get_nodes() const;
	
	unsigned int get_city_radius() const;
	unsigned int get_multi_rail_width() const;
	
	void serialize(ostream& output) const;
	static BoardGeometry deserialize(istream& input);
};

#endif
