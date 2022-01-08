#ifndef _DRAWING_DATA_H
#define _DRAWING_DATA_H

#include "board_geometry.h"
#include <istream>
#include <ostream>

using namespace std;

class DrawingData{
	BoardGeometry geometry;
public:
	DrawingData(const BoardGeometry& geometry);
	
	const BoardGeometry& get_geometry() const;
	
	void serialize(ostream& output) const;
	static DrawingData deserialize(istream& input);
};

#endif
