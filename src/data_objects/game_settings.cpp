#include "game_settings.h"

#include "serialization.h"

GameSettings::GameSettings(unsigned int rails_per_turn, unsigned int player_num, unsigned int total_points) :
	rails_per_turn(rails_per_turn),
	player_num(player_num),
	total_points(total_points) {}

unsigned int GameSettings::get_rails_per_turn() const{
	return rails_per_turn;
}
unsigned int GameSettings::get_player_num() const{
	return player_num;
}
unsigned int GameSettings::get_total_points() const{
	return total_points;
}

void GameSettings::serialize(ostream& output) const{
	write_raw(output, rails_per_turn);
	write_raw(output, player_num);
	write_raw(output, total_points);
}
GameSettings GameSettings::deserialize(istream& input){
	unsigned int rails_per_turn = read_raw<unsigned int>(input);
	unsigned int player_num = read_raw<unsigned int>(input);
	unsigned int total_points = read_raw<unsigned int>(input);
	
	return GameSettings(
		rails_per_turn,
		player_num,
		total_points
	);
}

