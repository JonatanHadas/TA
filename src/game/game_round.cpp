#include "game_round.h"

void GameRound::add_rail(int edge_index){
	state.add_rail(edge_index);
	Edge edge = state.get_board().get_edges().at(edge_index);
	connectivity.unify(edge.get_node_a(), edge.get_node_b());
}
void GameRound::end_turn(){
	state.set_current_player((state.get_current_player() + 1) % settings.get_player_num());
	for(auto entry: observers){
		entry.first->set_current_player(state.get_current_player());
	}
}

GameRound::GameRound(const GameBoard& board, const GameSettings& settings, const vector<vector<unsigned int>>& player_cities, const DrawingData& drawing_data, const map<Observer*, unsigned int>& observers) :
	state(board, vector<unsigned int>(), vector<unsigned int>(), 0),
	settings(settings),
	connectivity(state.get_board().get_node_num()),
	player_cities(player_cities),
	drawing_data(drawing_data),
	observers(observers){

	for(Edge edge: state.get_rail_edges()){
		connectivity.unify(edge.get_node_a(), edge.get_node_b());
	}
	
	for(auto entry: observers){
		entry.first->clear_board();
		if(entry.second < settings.get_player_num()){
			entry.first->reveal_player_cities(entry.second, player_cities[entry.second]);
		}
		entry.first->set_current_player(state.get_current_player());
	}
}

const GameState& GameRound::get_state() const{
	return state;
}
const GameSettings& GameRound::get_settings() const{
	return settings;
}
const vector<unsigned int>& GameRound::get_cities(unsigned int player) const{
	return player_cities[player];
}

bool GameRound::add_station(unsigned int node_index){
	if(state.get_stations().size() >= settings.get_player_num()) return false;  // All stations placed.
	
	if(node_index > state.get_board().get_node_num()) return false;  // Outside board.

	for(auto station: state.get_stations()){
		if(station == node_index) return false;  // Place taken.
	}
	
	state.add_station(node_index);
	for(auto entry: observers){
		entry.first->add_station(node_index);
	}
	end_turn();
	return true;
}
bool GameRound::play(const vector<unsigned int> edge_indices){
	unsigned int weight = 0;
	if(state.get_stations().size() < settings.get_player_num()) return false;  // Not all stations placed.
	
	DisjointSets new_connectivity = connectivity;

	for(auto new_rail: edge_indices){
		if(new_rail > state.get_board().get_edges().size()) return false;  // Outside board.
		for(auto rail: state.get_rails()){
			if(rail == new_rail) return false;  // Place taken.
		}
		
		Edge edge = state.get_board().get_edges().at(new_rail);
		unsigned int station_set = new_connectivity.get_set(state.get_stations().at(state.get_current_player()));
		if(
			station_set != new_connectivity.get_set(edge.get_node_a()) &&
			station_set != new_connectivity.get_set(edge.get_node_b())
			) return false;
		
		new_connectivity.unify(edge.get_node_a(), edge.get_node_b());
	}
	
	for(auto rail: edge_indices){
		add_rail(rail);
	}

	for(auto entry: observers){
		entry.first->play(edge_indices);
	}
	end_turn();
	return true;
}

bool GameRound::check_win(unsigned int player_index) const{
	if(state.get_stations().size() <= player_index) return false;  // Station not placed.
	
	unsigned int station_set = connectivity.get_set(state.get_stations()[player_index]);
		
	for(unsigned int i = 0; i < player_cities[player_index].size(); i++){
		unsigned int city = state.get_board().get_cities()[i][player_cities[player_index][i]];
		if(connectivity.get_set(city) != station_set) return false;
	}
	
	return true;
}
bool GameRound::check_win() const{
	for(unsigned int i = 0; i < settings.get_player_num(); i++){
		if(check_win(i)) return true;
	}
	return false;
}

void GameRound::add_observer(Observer* observer, unsigned int player_index){
	observer->initialize(state, settings, drawing_data);
	if(player_index < settings.get_player_num()){
		observer->reveal_player_cities(player_index, player_cities[player_index]);
	}
	observers[observer] = player_index;
}
void GameRound::remove_observer(Observer* observer){
	observers.erase(observer);
}

void GameRound::end_round(){
	for(auto entry: observers){
		entry.first->end_round();
		for(unsigned int i = 0; i < settings.get_player_num(); i++){
			entry.first->reveal_player_cities(i, player_cities[i]);
		}
	}
}
