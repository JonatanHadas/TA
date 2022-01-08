#ifndef _GAME_GUI_H
#define _GAME_GUI_H

#include "game_drawer.h"
#include "../game/player_interface.h"

#include <SDL.h>

class GameGui{
	GameDrawer drawer;
	PlayerInterface* interface;
	
	bool last_rail_pending;
	
	bool handle_key_event(const SDL_KeyboardEvent& event);
	bool handle_mouse_motion(const SDL_MouseMotionEvent& event);
	bool handle_mouse_button(const SDL_MouseButtonEvent& event);
	
	void commit_rails();
	void place_station(unsigned int mouse_x, unsigned int mouse_y);
	void add_rail(unsigned int mouse_x, unsigned int mouse_y);
public:
	GameGui();

	Observer& get_observer();
	void set_player_interface(PlayerInterface* interface);
	
	bool handle_event(const SDL_Event& event);
	bool update_screen(SDL_Renderer* renderer);
};

#endif
