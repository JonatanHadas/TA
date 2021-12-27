#ifndef _GUI_UTILS_H
#define _GUI_UTILS_H

#include <SDL.h>

class Texture{
	SDL_Texture* texture;
public:
	Texture(SDL_Renderer* renderer,
		unsigned int format, int access,
		int width, int height
		);
	
	Texture(const Texture& inst) = delete;
	Texture(Texture&& inst);
	
	Texture& operator=(const Texture& inst) = delete;
	Texture& operator=(Texture&& inst);
	
	~Texture();
	
	SDL_Texture* get();
};

void draw_circle(SDL_Renderer* renderer, int x, int y, double radius);

#endif
