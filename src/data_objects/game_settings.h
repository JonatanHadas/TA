#ifndef _GAME_SETTINGS_H
#define _GAME_SETTINGS_H

#include <istream>
#include <ostream>

using namespace std;

class GameSettings{
	unsigned int rails_per_turn;
	unsigned int player_num;
	unsigned int total_points;
	
public:
	GameSettings(unsigned int rails_per_turn, unsigned int player_num, unsigned int total_points);
	
	unsigned int get_rails_per_turn() const;
	unsigned int get_player_num() const;
	unsigned int get_total_points() const;
	
	void serialize(ostream& output) const;
	static GameSettings deserialize(istream& input);
};

#endif
