#include "gui/game_gui.h"
#include "net_game/game_client.h"

#include <iostream>

SDL_Window*  screen;
SDL_Renderer* renderer;

void close_rend(){
	SDL_DestroyRenderer(renderer);
}
void close_window(){
	SDL_DestroyWindow(screen);
}

int main(int argc, char** argv){
	if(enet_initialize() != 0){
		cerr << "Error while initializing enet" << endl;
		return 4;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	std::string host;
	
	unsigned int player;
	
	int result;
	if(argc < 2){
		cout << "Please enter server address: ";
		cin >> host;
		result = enet_address_set_host(&address, host.c_str());
	}
	else{
		result = enet_address_set_host(&address, argv[1]);
	}
	if(result < 0){
		cerr << "Error resolving address" << endl;
		return 4;
	}
	
	if(argc < 3){
		cout << "Please enter server port: ";
		cin >> address.port;
	}
	else{
		address.port = atoi(argv[2]);
	}
	
	if(argc < 4){
		cout << "please enter player number: ";
		cin >> player;
	}
	else{
		player = atoi(argv[3]);
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		cerr << "Error initializing SDL:" << endl << SDL_GetError() << endl;
		return 1;
	}
	atexit(SDL_Quit);

	screen = SDL_CreateWindow(
										"Transamerica",
										SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
										960, 640, 0);
										//0, 0,
										//SDL_WINDOW_FULLSCREEN_DESKTOP);

	if(screen == NULL){
		cerr << "Error while opening window:" << endl << SDL_GetError() << endl;
		return 2;
	}
	atexit(close_window);

	renderer = SDL_CreateRenderer(screen, -1,SDL_RENDERER_TARGETTEXTURE);
	
	if(renderer == NULL){
		cerr << "Error while opening renderer:" << endl << SDL_GetError() << endl;
		return 3;
	}
	atexit(close_rend);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	
	GameGui gui(player);
	GameClient client(address, player, gui.get_observer());
	gui.set_player_interface(&client);
	
	while(true){
		if(client.has_error()){
			cerr << "Network error" << endl;
			return 5;
		}
		
		if(gui.update_screen(renderer)){
			SDL_RenderPresent(renderer);
		}
		
		SDL_Event event;
				
		while(SDL_PollEvent(&event)){
			if(!gui.handle_event(event)) return 0;
		}
		
		client.handle_event(100);
	}
}