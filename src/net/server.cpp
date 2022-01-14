#include "server.h"
#include "network_consts.h"

void Server::disconnect_now(){
	for(auto entry: peers){
		enet_peer_disconnect_now(entry.second, (enet_uint32)DisconnectionType::DECONSTRUCTION);
	}
}

Server::Server(enet_uint16 port, size_t peerCount, size_t channelLimit) :
	host(get_address_any(port), peerCount, channelLimit) {}

Server::~Server(){
	disconnect_now();
}

Server::Server(Server&& server):
	host(move(server.host)),
	peers(move(server.peers)) {}

Server& Server::operator=(Server&& server){
	host = move(server.host);
	disconnect_now();
	peers = move(server.peers);
	server.peers = map<peer_id_t, ENetPeer*>();
	return *this;
}

void Server::send_packet(peer_id_t peer_id, const string& data, enet_uint8 channel_id, enet_uint32 flags){
	enet_peer_send(peers[peer_id], channel_id, create_packet(data, flags));
	enet_host_flush(host.get());
}

peer_id_t Server::add_new_peer(ENetPeer* peer){
	if(peers.size() == 0) {
		peers[0] = peer;
		peer->data = reinterpret_cast<void*>(0);
		return 0;
	}
	for(auto entry: peers){
		intptr_t id_candidate = entry.first + 1;
		if(id_candidate == -1) continue;
		if(peers.find(id_candidate) == peers.end()){
			peers[id_candidate] = peer;
			peer->data = reinterpret_cast<void*>(id_candidate);
			return id_candidate;
		}
	}
	return -1;
}

int Server::handle_event(enet_uint32 timeout){
	ENetEvent event;
	int result = enet_host_service(host.get(), &event, timeout);
	if(result > 0){
		peer_id_t peer_id;
		switch(event.type){
		case ENET_EVENT_TYPE_CONNECT:
			peer_id = add_new_peer(event.peer);
			if(peer_id == -1){
				enet_peer_disconnect(event.peer, (enet_uint32)DisconnectionType::UNWANTED);  // Shouldn't happen.
			}
			else{
				handle_connection(peer_id);
			}
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			peer_id = reinterpret_cast<peer_id_t>(event.peer->data);
			peers.erase(peer_id);
			handle_disconnection(peer_id);
			event.peer->data = NULL;
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			peer_id = reinterpret_cast<peer_id_t>(event.peer->data);
			handle_receive(peer_id, get_packet_data(event.packet), event.channelID);
			break;
		}
	}
	return result;
}

void Server::disconnect(peer_id_t peer_id){
	enet_peer_disconnect(peers[peer_id], (enet_uint32)DisconnectionType::UNWANTED);
}

void Server::disconnect(){
	for(auto entry: peers){
		enet_peer_disconnect_later(entry.second, (enet_uint32)DisconnectionType::SOFT);
	}
	
	ENetEvent event;
	while(enet_host_service(host.get(), &event, 3000) > 0){
		switch(event.type){
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_CONNECT:
			enet_peer_disconnect(event.peer, (enet_uint32)DisconnectionType::CLOSING);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			peers.erase(reinterpret_cast<peer_id_t>(event.peer->data));
			event.peer->data = NULL;
			if(peers.size() == 0) return;
			break;
		}
	}
}
