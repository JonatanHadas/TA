#ifndef _NETWORK_CONSTS
#define _NETWORK_CONSTS

enum class DisconnectionType : unsigned int{
	DECONSTRUCTION = 1,
	UNWANTED = 2,
	SOFT = 3,
	CLOSING = 4,
	CONNECTION_ERROR = 5
};

#endif
