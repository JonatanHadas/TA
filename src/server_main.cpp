#include "net_game/game_server.h"

#include <time.h>

#include <iostream>
#include <fstream>

int main(int argc, char** argv){
	srand(time(NULL));
	
	if(enet_initialize() != 0){
		cerr << "Error while initializing enet" << endl;
		return 4;
	}
	atexit(enet_deinitialize);
	
	enet_uint16 port;
	unsigned int player_num;
	string board_path = "data/boards/america";

	if(argc < 2){
		cout << "Please enter server port: ";
		cin >> port;
	}
	else{
		port = atoi(argv[1]);
	}

	if(argc < 3){
		cout << "Please enter player number: ";
		cin >> player_num;
	}
	else{
		player_num = atoi(argv[2]);
	}
	
	if(argc >= 4){
		board_path = argv[3];
	}
	
	ifstream board_file;
	board_file.open((board_path + "/board").c_str(), ios_base::binary);
	GameBoard board(GameBoard::deserialize(board_file));
	board_file.close();

	ifstream geometry_file;
	geometry_file.open((board_path+"/geometry").c_str(), ios_base::binary);
	BoardGeometry geometry(BoardGeometry::deserialize(geometry_file));
	geometry_file.close();
	
	GameSettings settings(
		2,
		player_num,
		12,
		2,
		2
	);
	
	GameServer server(
		port, 32,
		board, settings,
		DrawingData(geometry));
		
	while(server.handle_event(1000) >= 0);
	
	cerr << "Network error" << endl;
}