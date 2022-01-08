#include "client.h"
#include "network_consts.h"

void Client::disconnect_now(){
	if(NULL != peer) enet_peer_disconnect_now(peer, (enet_uint32)DisconnectionType::DECONSTRUCTION);
}

Client::Client(const ENetAddress& address, size_t channelLimit) :
	host(1, channelLimit),
	peer(NULL) {
			
	if(host.get() != NULL) peer = enet_host_connect(host.get(), &address, channelLimit, 0);
}

Client::~Client(){
	disconnect_now();
}

Client::Client(Client&& client) :
	host(move(client.host)),
	peer(client.peer) {
	
	client.peer = NULL;
}

Client& Client::operator=(Client&& client){
	host = move(client.host);
	
	disconnect_now();
	peer = client.peer;
	client.peer = NULL;
	return *this;
}

void Client::send_packet(const string& data, enet_uint8 channel_id, enet_uint32 flags){
	enet_peer_send(peer, channel_id, create_packet(data, flags));
	enet_host_flush(host.get());
}

int Client::handle_event(enet_uint32 timeout){
	ENetEvent event;
	int result = enet_host_service(host.get(), &event, timeout);
	if(result > 0){
		switch(event.type){
		case ENET_EVENT_TYPE_CONNECT:
			if(event.peer == peer){
				handle_connection();
			}
			else{
				enet_peer_disconnect(peer, (enet_uint32)DisconnectionType::UNWANTED);
			}
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			if(event.peer == peer){
				peer = NULL;
				handle_disconnection();
			}
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			if(event.peer == peer){
				handle_receive(get_packet_data(event.packet), event.channelID);
			}
			break;
		}
	}
	return result;
}

void Client::disconnect(){
	if(peer != NULL){
		enet_peer_disconnect_later(peer, (enet_uint32)DisconnectionType::SOFT);
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
			if(peer == event.peer){
				peer = NULL;
				return;
			}
			break;
		}
	}
}