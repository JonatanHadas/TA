#ifndef _GEOMETRY_H
#define _GEOMETRY_H

int leftness(
	int start_x, int start_y,
	int end_x, int end_y,
	int x, int y
	);
	
double distance(
	int x1, int y1,
	int x2, int y2
	);
	
double segment_distance(
	int start_x, int start_y,
	int end_x, int end_y,
	int x, int y
	);

#endif
