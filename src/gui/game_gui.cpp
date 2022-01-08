#include "game_gui.h"

#include "../utils/geometry.h"

GameGui::GameGui() :
	interface(NULL),
	last_rail_pending(false) {}

Observer& GameGui::get_observer(){
	return drawer.get_observer();
}
void GameGui::set_player_interface(PlayerInterface* interface){
	this->interface = interface;
}

bool GameGui::handle_event(const SDL_Event& event){
	switch(event.type){
	case SDL_QUIT:
		return false;
	case SDL_KEYDOWN:
		return handle_key_event(event.key);
	case SDL_MOUSEBUTTONDOWN:
		return handle_mouse_button(event.button);
	case SDL_MOUSEMOTION:
		return handle_mouse_motion(event.motion);
	}
	return true;
}

bool GameGui::handle_key_event(const SDL_KeyboardEvent& event){
	switch(event.keysym.sym){
	case SDLK_ESCAPE:
		return false;
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		if(drawer.get_observer().is_in_round()){
			if(drawer.get_observer().showing_last_round()){
				drawer.get_observer().clear_last_round();
			}
			else{
				commit_rails();
			}
		}
		break;
	case SDLK_BACKSPACE:
		drawer.get_observer().undo_rail();
		break;
	}
	return true;
}
bool GameGui::handle_mouse_motion(const SDL_MouseMotionEvent& event){
	if(drawer.get_observer().is_in_round() && !drawer.get_observer().showing_last_round()){
		int mouse_x = event.x;
		int mouse_y = event.y;
		drawer.unscale_point(mouse_x, mouse_y);
		if(drawer.get_observer().get_last_station() == drawer.get_observer().get_settings().get_player_num()){
			add_rail(mouse_x, mouse_y);
		}
		else{
			place_station(mouse_x, mouse_y);
		}
	}
	return true;
}

bool GameGui::handle_mouse_button(const SDL_MouseButtonEvent& event){
	if(event.button == SDL_BUTTON_LEFT){
		if(drawer.get_observer().is_in_round()){
			if(drawer.get_observer().showing_last_round()){
				drawer.get_observer().clear_last_round();
			}
			else{
				if(drawer.get_observer().get_last_station() < drawer.get_observer().get_settings().get_player_num()){
				if(drawer.get_observer().get_added_station() != drawer.get_observer().get_state().get_board().get_node_num())
					if(interface != NULL) interface->add_station(drawer.get_observer().get_added_station());
				}
			}
		}
		last_rail_pending = false;
	}
	return true;
}

void GameGui::commit_rails(){
	if(last_rail_pending){
		drawer.get_observer().undo_rail();
		last_rail_pending = false;
	}

	if(interface != NULL) interface->play(drawer.get_observer().get_added_rails());
}

void GameGui::place_station(unsigned int mouse_x, unsigned int mouse_y){
	const BoardGeometry& geometry = drawer.get_observer().get_drawing_data().get_geometry();

	unsigned int added_station = drawer.get_observer().get_added_station();
	double min_dist = 0;
	unsigned int node_num = drawer.get_observer().get_state().get_board().get_node_num();
	if(added_station < node_num){
		auto node = geometry.get_nodes().at(added_station);
		min_dist = distance(
			node.first, node.second,
			mouse_x, mouse_y
		);
	}
	for(unsigned int i = 0; i < node_num; i++){
		if(drawer.get_observer().check_station(i)){
			auto node = geometry.get_nodes().at(i);
			double dist = distance(
				node.first, node.second,
				mouse_x, mouse_y
			);
									
			if((added_station == node_num) || (dist < min_dist)){
				added_station = i;
				min_dist = dist;
				drawer.set_changed();
			}
		}
	}
	if(added_station != node_num){
		drawer.get_observer().place_station(added_station);
	}
}

void GameGui::add_rail(unsigned int mouse_x, unsigned int mouse_y){
	const BoardGeometry& geometry = drawer.get_observer().get_drawing_data().get_geometry();

	if(last_rail_pending) drawer.get_observer().undo_rail();
	unsigned int edge_num = drawer.get_observer().get_state().get_board().get_edges().size();
	unsigned int closest_edge = 0;
	unsigned int current_player = drawer.get_observer().get_state().get_current_player();
	double min_dist = 0;
	last_rail_pending = false;
	for(unsigned int edge_index = 0; edge_index < edge_num; edge_index ++){
		if(drawer.get_observer().check_rail(current_player, edge_index)){
			auto edge = drawer.get_observer().get_state().get_board().get_edges().at(edge_index);
			auto node_a = geometry.get_nodes().at(edge.get_node_a());
			auto node_b = geometry.get_nodes().at(edge.get_node_b());
			double dist = segment_distance(
				node_a.first, node_a.second,
				node_b.first, node_b.second,
				mouse_x, mouse_y
			);
											
			if(!last_rail_pending || dist < min_dist){
				min_dist = dist;
				closest_edge = edge_index;
			}

			last_rail_pending = true;
		}
	}
	if(last_rail_pending){
		drawer.get_observer().add_rail(closest_edge);
		drawer.set_changed();
	}	
}

bool GameGui::update_screen(SDL_Renderer* renderer){
	if(!drawer.is_changed()) return false;
	
	drawer.init(renderer);
	drawer.draw(renderer);
	drawer.set_unchanged();
	
	return true;
}
