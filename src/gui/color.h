#ifndef _COLORS_H
#define _COLORS_H

#include <SDL.h>

#include <vector>

using namespace std;

vector<SDL_Color> city_colors({
	{0, 255, 0, 0},
	{0, 64, 255, 0},
	{255, 64, 64, 0},
	{255, 192, 0, 0},
	{255, 255, 64, 0},
});

vector<SDL_Color> player_colors({
	{0, 192, 0, 0},
	{192, 0, 0, 0},
	{0, 0, 128, 0},
	{240, 220, 0, 0},
	{255, 255, 255, 0},
	{96, 64, 0, 0},	
});

#endif
