#ifndef _CLIENT_H
#define _CLIENT_H

#include "enet_wrapper.h"

class Client{
	NetHost host;
	ENetPeer* peer;
	void disconnect_now();
	
protected:
	virtual void handle_connection() = 0;
	virtual void handle_disconnection() = 0;
	virtual void handle_receive(string data, enet_uint8 channel_id) = 0;
public:
	Client(const ENetAddress& address, size_t channelLimit);
	
	~Client();
	
	Client(const Client&) = delete;
	Client(Client&& client);
	
	Client& operator=(const Client&) = delete;
	Client& operator=(Client&& client);
	
	void send_packet(const string& data, enet_uint8 channel_id, enet_uint32 flags);

	int handle_event(enet_uint32 timeout);
	
	void disconnect();
};

#endif
