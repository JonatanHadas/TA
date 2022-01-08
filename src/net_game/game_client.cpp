#include "game_client.h"
#include "game_protocol.h"

#include "../data_objects/serialization.h"
#include "../data_objects/game_state.h"
#include "../data_objects/game_settings.h"
#include "../data_objects/game_score.h"

#include <sstream>


GameClient::GameClient(const ENetAddress& address, unsigned int player, Observer& observer) :
	Client(address, 2),
	observer(observer) {
	
	ostringstream packet(ios_base::binary);
	write_raw(packet, player);
	
	send_packet(packet.str(), (enet_uint8)Channel::OBSERVING, ENET_PACKET_FLAG_RELIABLE);
}

bool GameClient::add_station(unsigned int node_index){
	ostringstream packet(ios_base::binary);
	
	write_raw(packet, PlayerInterfaceMessageType::ADD_STATION);
	write_raw(packet, node_index);
	
	send_packet(packet.str(), (enet_uint8)Channel::PLAYER, ENET_PACKET_FLAG_RELIABLE);
	
	return true;
}
bool GameClient::play(vector<unsigned int> edge_indices){
	ostringstream packet(ios_base::binary);
	
	write_raw(packet, PlayerInterfaceMessageType::PLAY);
	
	write_vector(packet, edge_indices, write_raw<unsigned int>);
	
	send_packet(packet.str(), (enet_uint8)Channel::PLAYER, ENET_PACKET_FLAG_RELIABLE);
	
	return true;	
}

void GameClient::handle_connection(){
	
}
void GameClient::handle_disconnection(){
	
}
void GameClient::handle_receive(string data, enet_uint8 channel_id){
	Channel channel = (Channel)channel_id;
	
	istringstream packet(data, ios_base::binary);
	switch(channel){
	case Channel::OBSERVING:
		switch(read_raw<ObserverMessageType>(packet)){
		case ObserverMessageType::INITIALIZE:
			{
				GameState state = GameState::deserialize(packet);
				GameSettings settings = GameSettings::deserialize(packet);
				DrawingData drawing_data = DrawingData::deserialize(packet);
				observer.initialize(state, settings, drawing_data);
			}
			break;
		case ObserverMessageType::CLEAR_BOARD:
			observer.clear_board();
			break;
		case ObserverMessageType::ADD_STATION:
			{
				auto station_index = read_raw<unsigned int>(packet);
				observer.add_station(station_index);
			}
			break;
		case ObserverMessageType::PLAY:
			{
				auto rails = read_vector<unsigned int>(packet, read_raw<unsigned int>);
				observer.play(rails);
			}
			break;
		case ObserverMessageType::SET_CURRENT_PLAYER:
			{
				auto player = read_raw<unsigned int>(packet);
				observer.set_current_player(player);
			}
			break;
		case ObserverMessageType::SET_STARTING_PLAYER:
			{
				auto player = read_raw<unsigned int>(packet);
				observer.set_starting_player(player);
			}
			break;
		case ObserverMessageType::SET_SCORES:
			{
				auto scores = GameScore::deserialize(packet);
				observer.set_scores(scores);
			}
			break;
		case ObserverMessageType::REVEAL_PLAYER_CITIES:
			{
				auto player = read_raw<unsigned int>(packet);
				auto cities = read_vector<unsigned int>(packet, read_raw<unsigned int>);
				observer.reveal_player_cities(player, cities);
			}
			break;
		case ObserverMessageType::END_ROUND:
			observer.end_round();
			break;
		}
		break;
	}
}
