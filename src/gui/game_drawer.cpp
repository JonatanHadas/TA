#include "game_drawer.h"

#include "gui_utils.h"
#include "color.h"

#include "../algorithms/disjoint_sets.h"

#define _USE_MATH_DEFINES
#include <math.h>

GameStateObserver::GameStateObserver(function<void()> update_callback) :
	update_callback(update_callback),
	in_round(false),
	last_rail(0),
	last_station(0)
	{}

void GameStateObserver::initialize(const GameState& state, const GameSettings& settings){
	this->state = make_unique<GameState>(state);
	this->settings = make_unique<GameSettings>(settings);
	in_round = true;
	last_rail = state.get_rails().size();
	last_station = state.get_stations().size();
	update_callback();
}
void GameStateObserver::clear_board(){
	state->clear();
	last_rail = state->get_rails().size();
	last_station = state->get_stations().size();
	in_round = true;
	player_cities.clear();
	update_callback();
}

void GameStateObserver::clear_rails(){
	while(last_rail > state->get_rails().size()) state->undo_rail();
}
void GameStateObserver::clear_stations(){
	while(last_station < state->get_stations().size()){
		state->undo_station();
	}
}

void GameStateObserver::add_station(unsigned int station_index){
	place_station(station_index);
	last_station = state->get_stations().size();
	update_callback();
}
void GameStateObserver::play(const vector<unsigned int>& rails){
	clear_rails();
	for(unsigned int rail: rails){
		add_rail(rail);
	}
	last_rail = state->get_rails().size();
	update_callback();
}
void GameStateObserver::set_current_player(unsigned int player){
	state->set_current_player(player);
	update_callback();
}

void GameStateObserver::reveal_player_cities(unsigned int player, const vector<unsigned int>& cities){
	player_cities[player] = cities;
	update_callback();
}
void GameStateObserver::end_round(){
	in_round = false;
}

const GameState& GameStateObserver::get_state() const{
	return *state;
}
const GameSettings& GameStateObserver::get_settings() const{
	return *settings;
}
const map<unsigned int, vector<unsigned int>> GameStateObserver::get_player_cities() const{
	return player_cities;
}
bool GameStateObserver::is_in_round() const{
	return in_round;
}

unsigned int GameStateObserver::get_last_station() const{
	return last_station;
}

void GameStateObserver::add_rail(unsigned int edge_index){
	state->add_rail(edge_index);
	update_callback();
}
void GameStateObserver::undo_rail(){
	if(last_rail < state->get_rails().size()) state->undo_rail();
}

void GameStateObserver::place_station(unsigned int node_index){
	clear_stations();
	state->add_station(node_index);
}

unsigned int GameStateObserver::get_added_station() const{
	if(last_station < state->get_stations().size()) return state->get_stations().back();
	return state->get_board().get_node_num();
}
vector<unsigned int> GameStateObserver::get_added_rails() const{
	vector<unsigned int> rails;
	for(unsigned int i = last_rail; i < state->get_rails().size(); i++){
		rails.push_back(state->get_rails().at(i));
	}
	return rails;
}

#include <iostream>
bool GameStateObserver::check_rail(unsigned int player, unsigned int edge_index) const{
	if(state->get_stations().size() < player) return false;
	
	unsigned int weight = state->get_board().get_edges().at(edge_index).get_weight();
	for(unsigned int rail: get_added_rails()){
		weight += state->get_board().get_edges().at(rail).get_weight();
	}
	if(weight > settings->get_rails_per_turn()) return false;

	DisjointSets connectivity(state->get_board().get_node_num());
	for(unsigned index: state->get_rails()){
		if(edge_index == index) return false;
		auto edge = state->get_board().get_edges().at(index);
		connectivity.unify(edge.get_node_a(), edge.get_node_b());
	}
	auto station_set = connectivity.get_set(state->get_stations().at(player));
	auto edge = state->get_board().get_edges().at(edge_index);
	return 
		station_set == connectivity.get_set(edge.get_node_a()) ||
		station_set == connectivity.get_set(edge.get_node_b());
}

bool GameStateObserver::check_station(unsigned int node_index) const{
	for(unsigned int i = 0; i < last_station; i++){
		if(state->get_stations().at(i) == node_index) return false;
	}
	return true;
}


GameDrawer::GameDrawer(BoardGeometry geometry) : 
	geometry(geometry),
	observer([this]() {this->changed = true;}),
	changed(false) {}

	void GameDrawer::init(SDL_Renderer* renderer){
	int output_w, output_h;
	SDL_GetRendererOutputSize(renderer, &output_w, &output_h);
	int scaled_w = output_h * geometry.get_width() / geometry.get_height();
	int scaled_h = output_w * geometry.get_height() / geometry.get_width();
	screen_width = max(output_w, scaled_w);
	screen_height = max(output_h, scaled_h);
	
	SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);
}
void GameDrawer::draw(SDL_Renderer* renderer) const{
	SDL_SetRenderDrawColor(renderer, 240, 240, 240, 0);
	SDL_RenderClear(renderer);
	
	SDL_Texture* orig_target = SDL_GetRenderTarget(renderer);

	Texture city(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		2 * geometry.get_city_radius(), 2 * geometry.get_city_radius()
		);
	Texture station(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		geometry.get_city_radius(), geometry.get_city_radius()
		);
	SDL_SetRenderTarget(renderer, city.get());
	SDL_SetTextureBlendMode(city.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	draw_circle(renderer, geometry.get_city_radius(), geometry.get_city_radius(), geometry.get_city_radius());
	
	SDL_SetRenderTarget(renderer, station.get());
	SDL_SetTextureBlendMode(station.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	draw_circle(renderer, 0.5 * geometry.get_city_radius(), 0.5 * geometry.get_city_radius(), 0.5 * geometry.get_city_radius());
	
	SDL_SetRenderTarget(renderer, orig_target);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	for(Edge edge: observer.get_state().get_board().get_edges()){
		auto node_a = geometry.get_nodes().at(edge.get_node_a());
		auto node_b = geometry.get_nodes().at(edge.get_node_b());
		
		double length = sqrt(
			(node_a.first - node_b.first) * (node_a.first - node_b.first) +
			(node_a.second - node_b.second) * (node_a.second - node_b.second)
			);
			
		pair<double, double> node_b2(
			((node_a.first * 0.5 * geometry.get_city_radius()) + (node_b.first * (length - 0.5 * geometry.get_city_radius()))) / length,
			((node_a.second * 0.5 * geometry.get_city_radius()) + (node_b.second * (length - 0.5 * geometry.get_city_radius()))) / length
			);
		pair<double, double> node_a2(
			((node_b.first * 0.5 * geometry.get_city_radius()) + (node_a.first * (length - 0.5 * geometry.get_city_radius()))) / length,
			((node_b.second * 0.5 * geometry.get_city_radius()) + (node_a.second * (length - 0.5 * geometry.get_city_radius()))) / length
			);
			
		pair<double, double> offset(
			(int)(node_b.second - node_a.second) / length * geometry.get_multi_rail_width() / edge.get_weight() / 2,
			(int)(node_a.first - node_b.first) / length * geometry.get_multi_rail_width() / edge.get_weight() / 2
			);

		if(edge.get_weight() % 2){
			int x1 = node_a.first, y1 = node_a.second;
			int x2 = node_b.first, y2 = node_b.second;
			
			scale_point(x1, y1);
			scale_point(x2, y2);
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		}
		else{
			int x1 = node_a.first, y1 = node_a.second;
			int x2 = node_a2.first, y2 = node_a2.second;
			
			scale_point(x1, y1);
			scale_point(x2, y2);
			SDL_RenderDrawLine(renderer, x1, y1 ,x2, y2);
			
			x1 = node_b.first; y1 = node_b.second;
			x2 = node_b2.first; y2 = node_b2.second;

			scale_point(x1, y1);
			scale_point(x2, y2);
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		}
		for(int i = 1 - edge.get_weight(); i < (int)edge.get_weight(); i += 2){
			if(i == 0) continue;
			
			int x1 = node_a2.first + i * offset.first, y1 = node_a2.second + i * offset.second;
			int x2 = node_b2.first + i * offset.first, y2 = node_b2.second + i * offset.second;
			
			scale_point(x1, y1);
			scale_point(x2, y2);
			SDL_RenderDrawLine(renderer, x2, y2, x1, y1);
		}
	}
		
	for(unsigned int i = 0; i < observer.get_state().get_board().get_cities().size(); i++){
		for(unsigned int node_index: observer.get_state().get_board().get_cities().at(i)){

			auto node = geometry.get_nodes().at(node_index);
			
			SDL_Rect src, dst;
			src.w = dst.w = src.h = dst.h = 2 * geometry.get_city_radius();
			src.x = src.y = 0;
			dst.x = node.first - dst.w / 2;
			dst.y = node.second - dst.h / 2;
			
			scale_point(dst.w, dst.h);
			scale_point(dst.x, dst.y);

			SDL_SetTextureColorMod(city.get(),
				city_colors[i].r,
				city_colors[i].g,
				city_colors[i].b
				);
			SDL_RenderCopy(renderer, city.get(), &src, &dst);
		}
	}

	for(auto entry: observer.get_player_cities()){
		unsigned int player_index = entry.first;
		for(unsigned int i = 0; i < entry.second.size(); i++){
			unsigned node_index = observer.get_state().get_board().get_cities().at(i).at(entry.second.at(i));
			auto node = geometry.get_nodes().at(node_index);
			
			SDL_Rect rect;
			rect.w = rect.h = geometry.get_city_radius();
			rect.x = node.first - rect.w / 2;
			rect.y = node.second - rect.h / 2;
			
			scale_point(rect.w, rect.h);
			scale_point(rect.x, rect.y);
			
			SDL_SetRenderDrawColor(renderer,
				player_colors[player_index].r,
				player_colors[player_index].g,
				player_colors[player_index].b,
				0
				);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	for(auto edge: observer.get_state().get_rail_edges()){
		auto node_a = geometry.get_nodes().at(edge.get_node_a());
		auto node_b = geometry.get_nodes().at(edge.get_node_b());
		
		double length = sqrt(
			(node_a.first - node_b.first) * (node_a.first - node_b.first) +
			(node_a.second - node_b.second) * (node_a.second - node_b.second)
			);
			
		SDL_Point center;
		center.x = (node_a.first + node_b.first) / 2;
		center.y = (node_a.second + node_b.second) / 2;
		
		SDL_Rect src, dst;
		src.w = dst.w = length - geometry.get_city_radius();
		src.w = dst.w = length - geometry.get_city_radius();
		src.w = dst.w = length - geometry.get_city_radius();
		src.h = dst.h = geometry.get_multi_rail_width();
		src.x = src.y = 0;
		dst.x = center.x - src.w / 2;
		dst.y = center.y - src.h / 2;
		
		double angle = atan2((int)(node_a.second - node_b.second), (int)(node_a.first - node_b.first)) * 180.0 / M_PI;
		
		scale_point(src.w, src.h);
		scale_point(dst.w, dst.h);
		scale_point(src.x, src.y);
		scale_point(dst.x, dst.y);
		scale_point(center.x, center.y);

		Texture rail(renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
			src.w, src.h);
		
		SDL_SetRenderTarget(renderer, rail.get());
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, orig_target);
		
		SDL_RenderCopyEx(renderer, rail.get(), &src, &dst, angle, NULL, SDL_FLIP_NONE);
	}
	
	for(unsigned int i = 0; i < observer.get_state().get_stations().size(); i++){
		unsigned int node_index = observer.get_state().get_stations().at(i);
		auto node = geometry.get_nodes().at(node_index);
		
		SDL_Rect src, dst;
		src.w = dst.w = src.h = dst.h = geometry.get_city_radius();
		src.x = src.y = 0;
		dst.x = node.first - dst.w / 2;
		dst.y = node.second - dst.h / 2;
		
		scale_point(dst.w, dst.h);
		scale_point(dst.x, dst.y);

		SDL_SetTextureColorMod(station.get(), 
			player_colors[i].r,
			player_colors[i].g,
			player_colors[i].b
			);
		SDL_RenderCopy(renderer, station.get(), &src, &dst);
	}
}
void GameDrawer::scale_point(int& x, int& y) const{
	x *= screen_width;
	x /= geometry.get_width();
	y *= screen_height;
	y /= geometry.get_height();
}
void GameDrawer::unscale_point(int& x, int& y) const{
	x *= geometry.get_width();
	x /= screen_width;
	y *= geometry.get_height();
	y /= screen_height;
}

bool GameDrawer::is_changed() const{
	return changed;
}

void GameDrawer::set_changed(){
	changed = true;
}

void GameDrawer::set_unchanged(){
	changed = false;
}

GameStateObserver& GameDrawer::get_observer(){
	return observer;
}
