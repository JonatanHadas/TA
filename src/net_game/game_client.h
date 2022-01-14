#ifndef _GAME_CLIENT_H
#define _GAME_CLIENT_H

#include "../net/client.h"
#include "../game/player_interface.h"
#include "../game/game_observer.h"

class GameClient : public Client, public PlayerInterface{
	Observer* observer;
	unsigned int player;
protected:
	void handle_connection();
	void handle_disconnection();
	void handle_receive(string data, enet_uint8 channel_id);
public:
	GameClient(const ENetAddress& address, unsigned int player, Observer& observer);
	
	bool add_station(unsigned int node_index);
	bool play(vector<unsigned int> edge_indices);
};

#endif