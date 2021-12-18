#ifndef _TREE_SPAN_H
#define _TREE_SPAN_H

#include <set>
#include <map>
#include <vector>
#include <utility>

using namespace std;

typedef vector<map<unsigned int, unsigned int>> graph_t;

unsigned int min_connection_size(const graph_t& graph, vector<unsigned int> nodes);

set<pair<unsigned int, unsigned int>> min_connection_tree(const graph_t& graph, vector<unsigned int> nodes);

#endif