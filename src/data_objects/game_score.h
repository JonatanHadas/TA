#ifndef _GAME_SCORE_H
#define _GAME_SCORE_H

#include <vector>

#include <istream>
#include <ostream>

using namespace std;

class GameScore{
	int threshold;
	vector<int> scores;
public:
	GameScore(int threshold, vector<int>&& scores);
	GameScore(int threshold, int score, unsigned player_num);
	GameScore(int score, unsigned player_num);
	
	const vector<int>& get_scores() const;
	int get_threshold() const;
	bool is_finished() const;
	
	void decrease(vector<unsigned int> amounts, unsigned int threshold_distance);
	
	void serialize(ostream& output) const;
	static GameScore deserialize(istream& input);
};

#endif
