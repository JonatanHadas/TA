#include "game_score.h"

#include "serialization.h"

GameScore::GameScore(int threshold, vector<int>&& scores) :
	threshold(threshold),
	scores(scores) {}
GameScore::GameScore(int threshold, int score, unsigned player_num) :
	GameScore(threshold, vector<int>(player_num, score)) {}
GameScore::GameScore(int score, unsigned player_num) :
	GameScore(0, score, player_num) {}

const vector<int>& GameScore::get_scores() const{
	return scores;
}
int GameScore::get_threshold() const{
	return threshold;
}
bool GameScore::is_finished() const{
	for(int score: scores){
		if(score < threshold) return true;
	}
	return false;
}

void GameScore::decrease(vector<unsigned int> amounts, unsigned int threshold_distance){
	int min_score = 0;
	bool first = true;
	for(unsigned int i = 0; i < scores.size(); i++){
		scores[i] -= amounts[i];
		if(first || scores[i] < min_score) min_score = scores[i];
		first = false;
	}
	min_score -= threshold_distance;
	if(min_score > threshold){
		threshold = min_score;
	}
}

void GameScore::serialize(ostream& output) const{
	write_raw(output, threshold);
	unsigned int score_num = scores.size();
	write_raw(output, score_num);
	for(int score: scores){
		write_raw(output, score);
	}
}
GameScore GameScore::deserialize(istream& input){
	int threshold = read_raw<int>(input);
	unsigned int score_num = read_raw<unsigned int>(input);
	vector<int> scores;
	for(unsigned int i = 0; i < score_num; i++){
		scores.push_back(read_raw<int>(input));
	}
	return GameScore(threshold, move(scores));
}
