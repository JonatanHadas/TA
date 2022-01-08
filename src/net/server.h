#ifndef _SERVER_H
#define _SERVER_H

#include <map>

#include "enet_wrapper.h"

typedef intptr_t peer_id_t;

class Server{
	NetHost host;
	
	map<peer_id_t, ENetPeer*> peers;
	void disconnect_now();
	peer_id_t add_new_peer(ENetPeer* peer);
protected:
	virtual void handle_connection(peer_id_t peer_id) = 0;
	virtual void handle_disconnection(peer_id_t peer_id) = 0;
	virtual void handle_receive(peer_id_t peer_id, string data, enet_uint8 channel_id) = 0;
public:
	Server(enet_uint16 port, size_t peerCount, size_t channelLimit);
	
	~Server();
	
	Server(const Server&) = delete;
	Server(Server&& server);
	
	Server& operator=(const Server&) = delete;
	Server& operator=(Server&& server);

	void send_packet(peer_id_t peer_id, const string& data, enet_uint8 channel_id, enet_uint32 flags);
	
	int handle_event(enet_uint32 timeout);
	
	void disconnect();
	void disconnect(peer_id_t peer_id);
};

#endif
