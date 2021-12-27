#include "geometry.h"

#include <math.h>

int leftness(
	int start_x, int start_y,
	int end_x, int end_y,
	int x, int y
	){
	
	return (end_x - start_x) * (y - start_y) - (end_y - start_y) * (x - start_x);
}

double distance(
	int x1, int y1,
	int x2, int y2
	){
		
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
	
double segment_distance(
	int start_x, int start_y,
	int end_x, int end_y,
	int x, int y
	){
	
	int dx = end_x - start_x;
	int dy = end_y - start_y;
	
	if(leftness(start_x, start_y, start_x + dy, start_y - dx, x, y) < 0){
		return distance(start_x, start_y, x, y);
	}
	if(leftness(end_x, end_y, end_x + dy, end_y - dx, x, y) > 0){
		return distance(end_x, end_y, x, y);
	}
	return abs(leftness(start_x, start_y, end_x, end_y, x, y) / distance(start_x, start_y, end_x, end_y));
}
