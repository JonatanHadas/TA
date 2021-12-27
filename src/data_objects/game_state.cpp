#include "game_state.h"

#include "serialization.h"

#include <vector>

using namespace std;

Edge::Edge(unsigned int node_a, unsigned int node_b, unsigned int weight) :
	node_a(node_a),
	node_b(node_b),
	weight(weight)
	{
}
	
unsigned int Edge::get_node_a() const{
	return node_a;
}
unsigned int Edge::get_node_b() const{
	return node_b;
}
unsigned int Edge::get_weight() const{
	return weight;
}
Edge Edge::reverse() const{
	return move(Edge(node_b, node_a, weight));
}

void Edge::serialize(ostream& output) const{
	write_raw(output, node_a);
	write_raw(output, node_b);
	write_raw(output, weight);
}
Edge Edge::deserialize(istream& input){
	unsigned int node_a, node_b, weight;
	node_a = read_raw<unsigned int>(input);
	node_b = read_raw<unsigned int>(input);
	weight = read_raw<unsigned int>(input);

	return move(Edge(node_a, node_b, weight));
}

GameBoard::GameBoard(unsigned int node_num, const vector<Edge>& edges, const vector<vector<unsigned int>>& cities) :
	node_num(node_num),
	edges(edges),
	cities(cities)
	{
}
	
void GameBoard::serialize(ostream& output) const{
	write_raw(output, node_num);
	
	unsigned int edge_num = edges.size();
	write_raw(output, edge_num);
	for(unsigned int i = 0; i < edge_num; i++){
		edges[i].serialize(output);
	}
	
	unsigned int city_group_num = cities.size();
	write_raw(output, city_group_num);
	for(unsigned int i = 0; i < city_group_num; i++){
		unsigned int city_num = cities[i].size();
		write_raw(output, city_num);
		for(unsigned int j = 0; j < city_num; j++){
			write_raw(output, cities[i][j]);
		}
	}
}

GameBoard GameBoard::deserialize(istream& input){
	unsigned int node_num = read_raw<unsigned int>(input);
	
	GameBoard board(node_num, vector<Edge>(), vector<vector<unsigned int>>());

	unsigned int edge_num = read_raw<unsigned int>(input);
	for(unsigned int i = 0; i < edge_num; i++){
		board.edges.push_back(Edge::deserialize(input));
	}

	unsigned int city_group_num = read_raw<unsigned int>(input);
	for(unsigned int i = 0; i < city_group_num; i++){
		unsigned int city_num = read_raw<unsigned int>(input);
		board.cities.push_back(vector<unsigned int>());
		for(unsigned int j = 0; j < city_num; j++){
			board.cities[i].push_back(read_raw<unsigned int>(input));
		}
	}
	
	return move(board);
}
	
unsigned int GameBoard::get_node_num() const{
	return node_num;
}
const vector<vector<unsigned int>>& GameBoard::get_cities() const{
	return cities;
}
const vector<Edge>& GameBoard::get_edges() const{
	return edges;
}
	
vector<Edge> GameBoard::get_edges(unsigned int node) const{
	vector<Edge> neighbors;
	
	for(unsigned int i; i < edges.size(); i++){
		if(edges[i].get_node_a() == node) neighbors.push_back(edges[i]);
		else if(edges[i].get_node_b() == node) neighbors.push_back(edges[i].reverse());
	}
	
	return move(neighbors);
}

graph_t GameBoard::get_graph() const{
	graph_t graph(node_num);
	
	for(Edge edge: edges){
		graph[edge.get_node_a()][edge.get_node_b()] = edge.get_weight();
		graph[edge.get_node_b()][edge.get_node_a()] = edge.get_weight();
	}
	
	return graph;
}



GameState::GameState(const GameBoard& board, const vector<unsigned int>& stations, const vector<unsigned int>& rails, unsigned int current_player) :
	board(board),
	stations(stations),
	rails(rails),
	current_player(current_player)
	{
}
	
void GameState::serialize(ostream& output) const{
	board.serialize(output);
	
	unsigned int station_num = stations.size();
	write_raw(output, station_num);
	for(unsigned int i = 0; i < station_num; i++){
		write_raw(output, stations[i]);
	}

	unsigned int rail_num = rails.size();
	write_raw(output, rail_num);
	for(unsigned int i = 0; i < rail_num; i++){
		write_raw(output, rails[i]);
	}
	
	write_raw(output, current_player);
}
GameState GameState::deserialize(istream& input){
	GameState state(GameBoard::deserialize(input), vector<unsigned int>(), vector<unsigned int>(), 0);
	
	unsigned int station_num = read_raw<unsigned int>(input);
	for(unsigned int i = 0; i < station_num; i++){
		state.stations.push_back(read_raw<unsigned int>(input));
	}
	
	unsigned int rail_num = read_raw<unsigned int>(input);
	for(unsigned int i = 0; i < rail_num; i++){
		state.rails.push_back(read_raw<unsigned int>(input));
	}
	
	state.current_player = read_raw<unsigned int>(input);
	
	return move(state);
}
	
const GameBoard& GameState::get_board() const{
	return board;
}
const vector<unsigned int>& GameState::get_stations() const{
	return stations;
}
const vector<unsigned int>& GameState::get_rails() const{
	return rails;
}
vector<Edge> GameState::get_rail_edges() const{
	vector<Edge> rail_edges;
	
	for(unsigned int i: rails){
		rail_edges.push_back(board.get_edges()[i]);
	}
	
	return move(rail_edges);
}

unsigned int GameState::get_current_player() const{
	return current_player;
}
void GameState::set_current_player(unsigned int player){
	current_player = player;
}

void GameState::add_rail(unsigned int edge_index){
	rails.push_back(edge_index);
}
void GameState::undo_rail(){
	rails.pop_back();
}
	
void GameState::add_station(unsigned int node_index){
	stations.push_back(node_index);
}
void GameState::undo_station(){
	stations.pop_back();
}

void GameState::clear(){
	stations.clear();
	rails.clear();
}

graph_t GameState::get_graph() const{
	graph_t graph = board.get_graph();
	
	for(Edge edge: get_rail_edges()){
		graph[edge.get_node_a()][edge.get_node_b()] = 0;
		graph[edge.get_node_b()][edge.get_node_a()] = 0;
	}
	
	return graph;
}
