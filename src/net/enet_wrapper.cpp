#include "enet_wrapper.h"

NetHost::NetHost(
	const ENetAddress* address, size_t peerCount,
	size_t channelLimit,
	enet_uint32 ingoingBandwidth, enet_uint32 outgoingBandwidth) :
	host(enet_host_create(address, peerCount, channelLimit, ingoingBandwidth, outgoingBandwidth)) {}
NetHost::NetHost(
	const ENetAddress& address, size_t peerCount,
	size_t channelLimit,
	enet_uint32 ingoingBandwidth, enet_uint32 outgoingBandwidth) :
	NetHost(&address, peerCount, channelLimit, ingoingBandwidth, outgoingBandwidth) {}
NetHost::NetHost(
	size_t peerCount,
	size_t channelLimit,
	enet_uint32 ingoingBandwidth, enet_uint32 outgoingBandwidth) :
	NetHost(NULL, peerCount, channelLimit, ingoingBandwidth, outgoingBandwidth) {}
NetHost::NetHost(
	const ENetAddress& address, size_t peerCount,
	size_t channelLimit) : 
	NetHost(address, peerCount, channelLimit, 0, 0) {}
NetHost::NetHost(
	size_t peerCount,
	size_t channelLimit) :
	NetHost(peerCount, channelLimit, 0, 0) {}
		
NetHost::~NetHost() {
	if(NULL != host) enet_host_destroy(host);
}
NetHost::NetHost(NetHost&& host) :
	host(host.host){	
	host.host = NULL;
}
	
NetHost& NetHost::operator=(NetHost&& host){
	this->host = host.host;
	host.host = NULL;
	return *this;
}

ENetHost* NetHost::get(){
	return host;
}

ENetAddress get_address_any(enet_uint16 port){
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = port;
	return address;
}

ENetPacket* create_packet(const string& data, enet_uint32 flags){
	return enet_packet_create(data.c_str(), data.size(), flags);
}
string get_packet_data(ENetPacket* packet){
	string data((char*)packet->data, packet->dataLength);
	enet_packet_destroy(packet);
	return data;
}

