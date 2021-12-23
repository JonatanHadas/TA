#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include <ostream>
#include <istream>

#include <vector>

#include "../algorithms/tree_span.h"

using namespace std;

class Edge{
	unsigned int node_a;
	unsigned int node_b;
	unsigned int weight;
public:
	Edge(unsigned int node_a, unsigned int node_b, unsigned int weight);
	
	unsigned int get_node_a() const;
	unsigned int get_node_b() const;
	unsigned int get_weight() const;
	
	Edge reverse() const;
	
	void serialize(ostream& output) const;
	static Edge deserialize(istream& input);
};

class GameBoard{
	unsigned int node_num;

	vector<Edge> edges;
	vector<vector<unsigned int>> cities;
	
public:
	GameBoard(unsigned int node_num, const vector<Edge>& edges, const vector<vector<unsigned int>>& cities);
	
	void serialize(ostream& output) const;
	static GameBoard deserialize(istream& input);
	
	unsigned int get_node_num() const;
	const vector<vector<unsigned int>>& get_cities() const;
	const vector<Edge>& get_edges() const;
	
	vector<Edge> get_edges(unsigned int node) const;

	graph_t get_graph() const;
};

class GameState{
	const GameBoard board;
	
	vector<unsigned int> stations;
	vector<unsigned int> rails;
	
	unsigned int current_player;
public:
	GameState(const GameBoard& board, const vector<unsigned int>& stations, const vector<unsigned int>& rails, unsigned int current_player);
	
	void serialize(ostream& output) const;
	static GameState deserialize(istream& input);
	
	const GameBoard& get_board() const;
	const vector<unsigned int>& get_stations() const;
	const vector<unsigned int>& get_rails() const;
	vector<Edge> get_rail_edges() const;
	
	unsigned int get_current_player() const;
	void set_current_player(unsigned int player);
	
	void add_rail(unsigned int edge_index);
	void undo_rail();
	
	void add_station(unsigned int node_index);
	void undo_station();
	
	void clear();
	
	graph_t get_graph() const;
};
	
#endif
