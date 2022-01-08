#ifndef _GAME_PROTOCOL_H
#define _GAME_PROTOCOL_H

enum class Channel : unsigned char{
	OBSERVING = 0,
	PLAYER = 1
};

enum class ObserverMessageType : char{
	INITIALIZE = 0,
	CLEAR_BOARD = 1,
	
	ADD_STATION = 2,
	PLAY = 3,
	
	SET_CURRENT_PLAYER = 4,
	SET_STARTING_PLAYER = 5,
	
	SET_SCORES = 6,
	REVEAL_PLAYER_CITIES = 7,
	END_ROUND = 8
};

enum class PlayerInterfaceMessageType : char{
	PLAY = 0,
	ADD_STATION = 1,
};

#endif
