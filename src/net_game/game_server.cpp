#include "game_server.h"
#include "game_protocol.h"

#include "../data_objects/serialization.h"

#include <sstream>

NetObserver::NetObserver(Server& server, peer_id_t peer_id) :
	server(&server),
	peer_id(peer_id) {}

void NetObserver::initialize(const GameState& state, const GameSettings& settings, const DrawingData& drawing_data){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::INITIALIZE);
	
	state.serialize(packet);
	settings.serialize(packet);
	drawing_data.serialize(packet);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}
void NetObserver::clear_board(){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::CLEAR_BOARD);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}

void NetObserver::add_station(unsigned int station_index){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::ADD_STATION);
	
	write_raw(packet, station_index);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}
void NetObserver::play(const vector<unsigned int>& rails){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::PLAY);
	
	write_vector(packet, rails, write_raw<unsigned int>);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}
void NetObserver::set_current_player(unsigned int player){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::SET_CURRENT_PLAYER);
	
	write_raw(packet, player);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}
void NetObserver::set_starting_player(unsigned int player){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::SET_STARTING_PLAYER);
	
	write_raw(packet, player);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}

void NetObserver::set_scores(const GameScore& scores){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::SET_SCORES);
	
	scores.serialize(packet);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}
void NetObserver::reveal_player_cities(unsigned int player, const vector<unsigned int>& cities){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::REVEAL_PLAYER_CITIES);
	
	write_raw(packet, player);
	write_vector(packet, cities, write_raw<unsigned int>);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}
void NetObserver::end_round(){
	ostringstream packet(ios_base::binary);
	write_raw(packet, ObserverMessageType::END_ROUND);
	
	server->send_packet(peer_id, packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}

GameServer::GameServer(enet_uint16 port, size_t peerCount, const GameBoard& board, const GameSettings& settings, const DrawingData& drawing_data) :
	game(board, settings, drawing_data, map<Observer*, unsigned int>()),
	Server(port, peerCount, 2) {}

void GameServer::handle_connection(peer_id_t peer_id){
	observers[peer_id] = make_unique<NetObserver>(*this, peer_id);
}
void GameServer::handle_disconnection(peer_id_t peer_id){
	if(players.find(peer_id) != players.end()){
		players.erase(peer_id);
		game.remove_observer(observers[peer_id].get());
	}
	observers.erase(peer_id);
}
void GameServer::handle_receive(peer_id_t peer_id, string data, enet_uint8 channel_id){
	Channel channel = (Channel)channel_id;
	
	istringstream packet(data, ios_base::binary);
	switch(channel){
	case Channel::OBSERVING:
		{
			auto player = read_raw<unsigned int>(packet);
			bool is_ok = true;
			for(auto entry: players){
				if(entry.first == peer_id) is_ok = false;
				if(entry.second == player){
					disconnect(peer_id);
					is_ok = false;
				}
			}
			if(is_ok){
				players[peer_id] = player;
				game.add_observer(observers[peer_id].get(), player);
			}
		}
		break;
	case Channel::PLAYER:
		{
			bool result = false;
			switch(read_raw<PlayerInterfaceMessageType>(packet)){
			case PlayerInterfaceMessageType::ADD_STATION:
				{
					auto node_index = read_raw<unsigned int>(packet);
					if(players.find(peer_id) != players.end())
						result = game.add_station(players[peer_id], node_index);
				}
				break;
			case PlayerInterfaceMessageType::PLAY:
				{
					auto edge_indices = read_vector<unsigned int>(packet, read_raw<unsigned int>);
					if(players.find(peer_id) != players.end())
						result = game.play(players[peer_id], edge_indices);
				}
				break;
			}
			ostringstream packet;
			write_raw(packet, result);
			send_packet(peer_id, packet.str(), (enet_uint8)Channel::PLAYER, ENET_PACKET_FLAG_RELIABLE);
		}
		break;
	}
}
