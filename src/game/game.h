#ifndef _GAME_H
#define _GAME_H

#include <memory>

#include "game_round.h"

#include "../data_objects/game_score.h"

class Game : public PlayerInterface{
	unsigned int starting_player;
	unsigned int round_num;
	GameSettings settings;
	GameScore score;
	GameBoard board;
	unique_ptr<GameRound> round;
	
	map<Observer*, unsigned int> observers;
	
	void new_round();
	void check_end_round();
	
	unsigned int state_player(unsigned int real_player) const;
public:
	Game(const GameBoard& board, const GameSettings& settings, map<Observer*, unsigned int>&& observers);

	bool add_station(unsigned int node_index);
	bool play(const vector<unsigned int> edge_indices);

	bool add_station(unsigned int player, unsigned int node_index);
	bool play(unsigned int player, const vector<unsigned int> edge_indices);
	
	void add_observer(Observer* observer, unsigned int player_index);	
	
	const GameRound* get_round() const;
	const GameSettings& get_settings() const;
	const GameScore& get_score() const;
};

#endif
