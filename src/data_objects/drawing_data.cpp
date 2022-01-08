#include "drawing_data.h"

DrawingData::DrawingData(const BoardGeometry& geometry) :
	geometry(geometry) {}

const BoardGeometry& DrawingData::get_geometry() const{
	return geometry;
}

void DrawingData::serialize(ostream& output) const{
	geometry.serialize(output);
}
DrawingData DrawingData::deserialize(istream& input){
	auto geometry = BoardGeometry::deserialize(input);
	return DrawingData(
		geometry
		);
}
