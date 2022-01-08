#ifndef _ENET_WRAPPER_H
#define _ENET_WRAPPER_H

#include <string>

#include "enet/enet.h"

using namespace std;

class NetHost;

class NetHost{
	ENetHost* host;
	NetHost(
		const ENetAddress* address, size_t peerCount,
		size_t channelLimit,
		enet_uint32 ingoingBandwidth, enet_uint32 outgoingBandwidth);
public:
	NetHost(
		const ENetAddress& address, size_t peerCount,
		size_t channelLimit,
		enet_uint32 ingoingBandwidth, enet_uint32 outgoingBandwidth);
	NetHost(
		size_t peerCount,
		size_t channelLimit,
		enet_uint32 ingoingBandwidth, enet_uint32 outgoingBandwidth);
	NetHost(
		const ENetAddress& address, size_t peerCount,
		size_t channelLimit);
	NetHost(
		size_t peerCount,
		size_t channelLimit);
		
	~NetHost();
	NetHost(const NetHost&) = delete;
	NetHost(NetHost&& host);
	
	NetHost& operator=(const NetHost&) = delete;
	NetHost& operator=(NetHost&& host);
	
	ENetHost* get();
};

ENetAddress get_address_any(enet_uint16 port);

ENetPacket* create_packet(const string& data, enet_uint32 flags);
string get_packet_data(ENetPacket* packet);

#endif
