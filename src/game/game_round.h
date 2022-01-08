#ifndef _GAME_ROUND_H
#define _GAME_ROUND_H

#include "../data_objects/game_state.h"
#include "../data_objects/game_settings.h"

#include "../algorithms/disjoint_sets.h"
#include "../algorithms/tree_span.h"

#include "player_interface.h"
#include "game_observer.h"

class GameRound : public PlayerInterface{
	GameState state;
	const GameSettings settings;
	
	const vector<vector<unsigned int>> player_cities;
	
	map<Observer*, unsigned int> observers;

	DisjointSets connectivity;

	void add_rail(int edge_index);
	void end_turn();
public:
	GameRound(const GameBoard& board, const GameSettings& settings, const vector<vector<unsigned int>>& player_cities, const map<Observer*, unsigned int>& observers);
	
	const GameState& get_state() const;
	const GameSettings& get_settings() const;
	const vector<unsigned int>& get_cities(unsigned int player) const;
	
	bool add_station(unsigned int node_index);
	bool play(const vector<unsigned int> edge_indices);
	
	bool check_win(unsigned int player_index) const;
	bool check_win() const;

	void add_observer(Observer* observer, unsigned int player_index);
	void remove_observer(Observer* observer);
	void end_round();
};

#endif
