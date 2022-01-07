#ifndef _BOARD_GEOMETRY_H
#define _BOARD_GEOMETRY_H

#include <vector>
#include <utility>

#include <ostream>
#include <istream>

using namespace std;

typedef pair<unsigned int, unsigned int> point_t;

class BoardGeometry{
	unsigned int width;
	unsigned int height;
	
	vector<point_t> nodes;
	
	pair<point_t, point_t> score_line;
	
	unsigned int city_radius;
	unsigned multi_rail_width;
public:
	BoardGeometry(
		unsigned int width, unsigned int height,
		vector<point_t> nodes,
		unsigned int city_radius, unsigned int multi_rail_width,
		pair<point_t, point_t> score_line
		);
		
	unsigned int get_width() const;
	unsigned int get_height() const;
	
	const vector<point_t>& get_nodes() const;
	
	const pair<point_t, point_t>& get_score_line() const;
	
	unsigned int get_city_radius() const;
	unsigned int get_multi_rail_width() const;
	
	void serialize(ostream& output) const;
	static BoardGeometry deserialize(istream& input);
};

#endif
