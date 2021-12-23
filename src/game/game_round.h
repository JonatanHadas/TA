#ifndef _GAME_ROUND_H
#define _GAME_ROUND_H

#include "../data_objects/game_state.h"
#include "../data_objects/game_settings.h"

#include "../algorithms/disjoint_sets.h"
#include "../algorithms/tree_span.h"

class GameRound{
	GameState state;
	const GameSettings settings;
	
	const vector<vector<unsigned int>> player_cities;

	DisjointSets connectivity;

	void add_rail(int edge_index);
	void end_turn();
public:
	GameRound(const GameState& state, const GameSettings& settings, const vector<vector<unsigned int>>& player_cities);
	
	const GameState& get_state() const;
	const GameSettings& get_settings() const;
	const vector<unsigned int>& get_cities(unsigned int player) const;
	
	bool place_station(unsigned int node_index);
	bool play(const vector<unsigned int> edge_indices);
	
	bool check_win(unsigned int player_index) const;
};

#endif
