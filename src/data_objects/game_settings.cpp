#include "game_settings.h"

#include "serialization.h"

GameSettings::GameSettings(unsigned int rails_per_turn, unsigned int player_num, unsigned int total_points, unsigned int rounds_to_threshold_move, unsigned int threshold_move_distance) :
	rails_per_turn(rails_per_turn),
	player_num(player_num),
	total_points(total_points),
	rounds_to_threshold_move(rounds_to_threshold_move),
	threshold_move_distance(threshold_move_distance) {}

unsigned int GameSettings::get_rails_per_turn() const{
	return rails_per_turn;
}
unsigned int GameSettings::get_player_num() const{
	return player_num;
}
unsigned int GameSettings::get_total_points() const{
	return total_points;
}
unsigned int GameSettings::get_rounds_to_threshold_move() const{
	return rounds_to_threshold_move;
}
unsigned int GameSettings::get_threshold_move_distance() const{
	return threshold_move_distance;
}

void GameSettings::serialize(ostream& output) const{
	write_raw(output, rails_per_turn);
	write_raw(output, player_num);
	write_raw(output, total_points);
	write_raw(output, rounds_to_threshold_move);
	write_raw(output, threshold_move_distance);
}
GameSettings GameSettings::deserialize(istream& input){
	unsigned int rails_per_turn = read_raw<unsigned int>(input);
	unsigned int player_num = read_raw<unsigned int>(input);
	unsigned int total_points = read_raw<unsigned int>(input);
	unsigned int rounds_to_threshold_move = read_raw<unsigned int>(input);
	unsigned int threshold_move_distance = read_raw<unsigned int>(input);
	
	return GameSettings(
		rails_per_turn,
		player_num,
		total_points,
		rounds_to_threshold_move,
		threshold_move_distance
	);
}

