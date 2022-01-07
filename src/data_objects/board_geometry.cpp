#include "board_geometry.h"

#include "serialization.h"

BoardGeometry::BoardGeometry(
	unsigned int width, unsigned int height,
	vector<point_t> nodes,
	unsigned int city_radius, unsigned int multi_rail_width,
	pair<point_t, point_t> score_line
	) :
	width(width),
	height(height),
	nodes(nodes),
	city_radius(city_radius),
	multi_rail_width(multi_rail_width),
	score_line(score_line) {}
	
unsigned int BoardGeometry::get_width() const{
	return width;
}
unsigned int BoardGeometry::get_height() const{
	return height;
}

const vector<point_t>& BoardGeometry::get_nodes() const{
	return nodes;
}

unsigned int BoardGeometry::get_city_radius() const{
	return city_radius;
}
unsigned int BoardGeometry::get_multi_rail_width() const{
	return multi_rail_width;
}

const pair<point_t, point_t>& BoardGeometry::get_score_line() const{
	return score_line;
}

void BoardGeometry::serialize(ostream& output) const{
	write_raw(output, width);
	write_raw(output, height);
	write_raw(output, city_radius);
	write_raw(output, multi_rail_width);

	write_raw(output, score_line.first.first);
	write_raw(output, score_line.first.second);
	write_raw(output, score_line.second.first);
	write_raw(output, score_line.second.second);
	
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
	
	pair<point_t, point_t> score_line;
	score_line.first.first = read_raw<unsigned int>(input);
	score_line.first.second = read_raw<unsigned int>(input);
	score_line.second.first = read_raw<unsigned int>(input);
	score_line.second.second = read_raw<unsigned int>(input);
	
	BoardGeometry geometry(
		width, height,
		vector<point_t>(),
		city_radius, multi_rail_width,
		score_line
		);
	
	unsigned int node_num = read_raw<unsigned int>(input);
	for(unsigned int i = 0; i < node_num; i++){
		unsigned int x = read_raw<unsigned int>(input);
		unsigned int y = read_raw<unsigned int>(input);
		geometry.nodes.push_back({x, y});
	}
	
	return geometry;
}
