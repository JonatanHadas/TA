#ifndef _GAME_OBSERVER_H
#define _GAME_OBSERVER_H

#include "../data_objects/game_state.h"
#include "../data_objects/game_settings.h"

class Observer{
public:
	virtual void initialize(const GameState& state, const GameSettings& settings) = 0;
	virtual void clear_board() = 0;
	
	virtual void add_station(unsigned int station_index) = 0;
	virtual void play(const vector<unsigned int>& rails) = 0;
	virtual void set_current_player(unsigned int player) = 0;
	
	virtual void reveal_player_cities(unsigned int player, const vector<unsigned int>& cities) = 0;
	virtual void end_round() = 0;
};

#endif
