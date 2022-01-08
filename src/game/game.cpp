#include "game.h"

#include <stdlib.h>

#include "../algorithms/tree_span.h"

unsigned int Game::state_player(unsigned int real_player) const{
	return (real_player + settings.get_player_num() - starting_player) % settings.get_player_num();
}

void Game::new_round(){
	if(NULL != round.get()) return;
	vector<vector<unsigned int>> player_cities(settings.get_player_num());
		
	for(auto city_group: board.get_cities()){
		vector<unsigned int> indices;
		for(unsigned int i = 0; i < city_group.size(); i++) indices.push_back(i);
		
		for(unsigned int player = 0; player < settings.get_player_num(); player++){
			unsigned int i = rand() % indices.size();
			player_cities[player].push_back(indices[i]);
			indices[i] = indices.back();
			indices.pop_back();
		}
	};
	
	map<Observer*, unsigned int> shifted_observers;
	for(auto entry: observers){
		shifted_observers[entry.first] = state_player(entry.second);
	}
	
	round = make_unique<GameRound>(board, settings, player_cities, shifted_observers);
}

void Game::check_end_round(){
	if(NULL == round.get()) return;
	
	if(round->check_win()){
		round->end_round();
		round_num++;
		
		vector<unsigned int> round_scores;
		const graph_t graph = round->get_state().get_graph();
		
		for(unsigned int player = 0; player < settings.get_player_num(); player++){
			vector<unsigned int> cities = round->get_cities(player);
			vector<unsigned int> nodes;
			for(unsigned int city_group = 0; city_group < cities.size(); city_group++){
				nodes.push_back(round->get_state().get_board().get_cities().at(city_group).at(cities[city_group]));
			}
			if(round->get_state().get_stations().size() > player) nodes.push_back(round->get_state().get_stations().at(player));
			
			round_scores.push_back(min_connection_size(graph, move(nodes)));
		}
		
		score.decrease(
			round_scores,
			round_num < settings.get_rounds_to_threshold_move() ? settings.get_total_points() : settings.get_threshold_move_distance()
			);

		for(auto entry: observers){
			entry.first->set_scores(score);
		}
		
		round = NULL;
		if(!score.is_finished()){
			new_round();

			starting_player++;
			starting_player %= settings.get_player_num();
			
			for(auto entry: observers){
				entry.first->set_starting_player(starting_player);
			}
		}
	}
}

Game::Game(const GameBoard& board, const GameSettings& settings, map<Observer*, unsigned int>&& observers) :
	settings(settings),
	score(settings.get_total_points(), settings.get_player_num()),
	board(board),
	starting_player(0),
	round_num(0),
	observers(observers){
	
	for(auto entry: observers){
		entry.first->set_scores(score);
		entry.first->set_starting_player(starting_player);
	}
	
	new_round();
}

bool Game::add_station(unsigned int node_index){
	if(NULL == round.get()) return false;
	bool result = round->add_station(node_index);
	
	if(result){
		check_end_round();
	}
	
	return result;
}
bool Game::play(const vector<unsigned int> edge_indices){
	if(NULL == round.get()) return false;
	bool result = round->play(edge_indices);
	
	if(result){
		check_end_round();
	}
	
	return result;
}

bool Game::add_station(unsigned int player, unsigned int node_index){
	if(NULL == round.get()) return false;
	if(round->get_state().get_current_player() != state_player(player)) return false;
	return add_station(node_index);
}
bool Game::play(unsigned int player, const vector<unsigned int> edge_indices){
	if(NULL == round.get()) return false;
	if(round->get_state().get_current_player() != state_player(player)) return false;
	return play(edge_indices);
}

void Game::add_observer(Observer* observer, unsigned int player_index){
	observer->set_scores(score);
	observer->set_starting_player(starting_player);
	
	if(NULL != round.get()) round->add_observer(observer, state_player(player_index));
	else{
		observer->initialize(GameState(board, vector<unsigned int>(), vector<unsigned int>(), 0), settings);
		observer->end_round();
	}
	observers[observer] = player_index;
}

void Game::remove_observer(Observer* observer){
	observers.erase(observer);
}

const GameRound* Game::get_round() const{
	return round.get();
}
const GameSettings& Game::get_settings() const{
	return settings;
}
const GameScore& Game::get_score() const{
	return score;
}
