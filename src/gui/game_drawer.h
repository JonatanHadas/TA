#ifndef _GAME_DRAWER_H
#define _GAME_DRAWER_H

#include <functional>
#include <memory>

#include <SDL.h>

#include "../game/player_interface.h"
#include "../game/game_observer.h"
#include "../data_objects/game_state.h"
#include "../data_objects/game_settings.h"
#include "../data_objects/board_geometry.h"

class GameStateObserver : public Observer{
	unique_ptr<GameState> state;
	unique_ptr<GameSettings> settings;
	unsigned int last_rail, last_station;
	bool in_round;
		
	function<void()> update_callback;
	
	map<unsigned int, vector<unsigned int>> player_cities;
	
	void clear_rails();
	void clear_stations();
public:
	GameStateObserver(function<void()> update_callback);

	void initialize(const GameState& state, const GameSettings& settings);
	void clear_board();
	
	void add_station(unsigned int station_index);
	void play(const vector<unsigned int>& rails);
	void set_current_player(unsigned int player);
	
	void reveal_player_cities(unsigned int player, const vector<unsigned int>& cities);
	void end_round();
	
	const GameState& get_state() const;
	const GameSettings& get_settings() const;
	const map<unsigned int, vector<unsigned int>> get_player_cities() const;
	bool is_in_round() const;
	unsigned int get_last_station() const;
		
	void add_rail(unsigned int edge_index);
	void undo_rail();
	
	void place_station(unsigned int node_index);
	
	unsigned int get_added_station() const;
	vector<unsigned int> get_added_rails() const;
	
	bool check_rail(unsigned int player, unsigned int edge_index) const;
	bool check_station(unsigned int node_index) const;
};

class GameDrawer{
	GameStateObserver observer;
	BoardGeometry geometry;
	bool changed;
	int screen_width, screen_height;
public:
	GameDrawer(BoardGeometry geometry);
	
	void init(SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer) const;
	
	void scale_point(int& x, int& y) const;
	void unscale_point(int& x, int& y) const;

	bool is_changed() const;
	void set_changed();
	void set_unchanged();
		
	GameStateObserver& get_observer();
};

#endif
