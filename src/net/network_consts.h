#ifndef _NETWORK_CONSTS
#define _NETWORK_CONSTS

enum class DisconnectionType : unsigned int{
	DECONSTRUCTION = 0,
	UNWANTED = 1,
	SOFT = 2,
	CLOSING = 3
};

#endif
