#ifndef _GAME_SERVER_H
#define _GAME_SERVER_H

#include "../net/server.h"
#include "../game/game.h"

#include <map>
#include <memory>

class NetObserver : public Observer {
	Server* server;
	peer_id_t peer_id;
public:
	NetObserver(Server& server, peer_id_t peer_id);

	void initialize(const GameState& state, const GameSettings& settings, const DrawingData& drawing_data);
	void clear_board();
	
	void add_station(unsigned int station_index);
	void play(const vector<unsigned int>& rails);
	void set_current_player(unsigned int player);
	void set_starting_player(unsigned int player);
	
	void set_scores(const GameScore& scores);
	void reveal_player_cities(unsigned int player, const vector<unsigned int>& cities);
	void end_round();
};

class GameServer : public Server{
	map<peer_id_t, unique_ptr<NetObserver>> observers;
	map<peer_id_t, unsigned int> players;
	
	Game game;
protected:
	void handle_connection(peer_id_t peer_id);
	void handle_disconnection(peer_id_t peer_id);
	void handle_receive(peer_id_t peer_id, string data, enet_uint8 channel_id);
public:
	GameServer(enet_uint16 port, size_t peerCount, const GameBoard& board, const GameSettings& settings, const DrawingData& drawing_data);
};

#endif
