#include "board_geometry.h"

#include "serialization.h"

BoardGeometry::BoardGeometry(
	unsigned int width, unsigned int height,
	vector<pair<unsigned int, unsigned int>> nodes,
	unsigned int city_radius, unsigned int multi_rail_width
	) :
	width(width),
	height(height),
	nodes(nodes),
	city_radius(city_radius),
	multi_rail_width(multi_rail_width) {}
	
unsigned int BoardGeometry::get_width() const{
	return width;
}
unsigned int BoardGeometry::get_height() const{
	return height;
}

const vector<pair<unsigned int, unsigned int>>& BoardGeometry::get_nodes() const{
	return nodes;
}

unsigned int BoardGeometry::get_city_radius() const{
	return city_radius;
}
unsigned int BoardGeometry::get_multi_rail_width() const{
	return multi_rail_width;
}

void BoardGeometry::serialize(ostream& output) const{
	write_raw(output, width);
	write_raw(output, height);
	write_raw(output, city_radius);
	write_raw(output, multi_rail_width);
	unsigned int node_num = nodes.size();
	write_raw(output, node_num);
	for(auto node: nodes){
		write_raw(output, node.first);
		write_raw(output, node.second);
	}
}

BoardGeometry BoardGeometry::deserialize(istream& input){
	unsigned int width = read_raw<unsigned int>(input);
	unsigned int height = read_raw<unsigned int>(input);
	unsigned int city_radius = read_raw<unsigned int>(input);
	unsigned int multi_rail_width = read_raw<unsigned int>(input);
	BoardGeometry geometry(
		width, height,
		vector<pair<unsigned int, unsigned int>>(),
		city_radius, multi_rail_width
		);
	
	unsigned int node_num = read_raw<unsigned int>(input);
	for(unsigned int i = 0; i < node_num; i++){
		unsigned int x = read_raw<unsigned int>(input);
		unsigned int y = read_raw<unsigned int>(input);
		geometry.nodes.push_back({x, y});
	}
	
	return geometry;
}
