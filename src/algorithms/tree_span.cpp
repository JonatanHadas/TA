#include "tree_span_internal.h"

#include "subset.h"

#include <queue>
#include <algorithm>

struct NodeDistance{
	unsigned int node;
	unsigned int distance;
};

vector<unsigned int> get_dist_map(const graph_t& graph, const map<unsigned int, unsigned int>& starting_points){
	vector<unsigned int> distances(graph.size(), 0);
	vector<bool> found(graph.size(), false);
	
	auto comp = [](const NodeDistance& node1, const NodeDistance& node2){
		return node1.distance > node2.distance;
	};
	
	priority_queue<NodeDistance, vector<NodeDistance>, decltype(comp)> heap(comp);
	
	for(auto entry: starting_points){
		NodeDistance node;
		node.node = entry.first;
		node.distance = entry.second;
		heap.push(node);
	}
	
	while(heap.size()){
		NodeDistance node = heap.top();
		heap.pop();
		if(found[node.node]) continue;

		found[node.node] = true;
		distances[node.node] = node.distance;
		for(auto neighbor: graph[node.node]){
			NodeDistance neighbor_distance;
			neighbor_distance.node = neighbor.first;
			neighbor_distance.distance = neighbor.second + node.distance;
			
			heap.push(neighbor_distance);
		}
	}
	
	return move(distances);
}

TreeNode::TreeNode(const vector<TreeNode>* contianer, unsigned int value) : 
	container(container),
	leaf(true),
	value(value)
	{
}
TreeNode::TreeNode(const vector<TreeNode>* container, unsigned int left, unsigned int right) :
	container(container),
	leaf(false),
	left(left),
	right(right){
}

bool TreeNode::is_leaf() const{
	return leaf;
}
set<unsigned int> TreeNode::get_leaves() const{
	if(leaf){
		return move(set<unsigned int>({value}));
	}
	set<unsigned int> leaves(get_left().get_leaves());
	set<unsigned int> right_leaves = get_right().get_leaves();
	leaves.insert(right_leaves.begin(), right_leaves.end());
	return move(leaves);
}

unsigned int TreeNode::get_value() const{
	return value;
}
const TreeNode& TreeNode::get_left() const{
	return (*container)[left];
}
const TreeNode& TreeNode::get_right() const{
	return (*container)[right];
}

int tree_compare(const TreeNode& first, const TreeNode& second){
	if(first.is_leaf() && second.is_leaf()){
		if(first.get_value() < second.get_value()) return -1;
		if(first.get_value() > second.get_value()) return 1;
		return 0;
	}
	
	int cmp = tree_compare(
		first.is_leaf() ? first : first.get_left(),
		second.is_leaf() ? second : second.get_left()
		);
	if(cmp) return cmp;
	
	return tree_compare(
		first.is_leaf() ? first : first.get_right(),
		second.is_leaf() ? second : second.get_right()
		);
}

unique_ptr<vector<TreeNode>> get_trees(const vector<unsigned int>& nodes){
	map<vector<bool>, pair<unsigned int, unsigned int>> groups;
	auto trees = make_unique<vector<TreeNode>>();
	
	for(vector<bool> current_nodes: SubsetGroup(nodes.size())){
		unsigned int start = trees->size();
		
		vector<unsigned int> node_indices = SubsetMask(current_nodes).get_indices();
		if(node_indices.size() == 1){
			trees->push_back(TreeNode(trees.get(), nodes[node_indices[0]]));
		}
		else{
			SubsetGroup subsets(current_nodes);
			for(vector<bool> subset: subsets){
				auto first_group = groups[subset];
				auto second_group = groups[subsets.inverse(subset)];
				if(first_group.first < second_group.first) continue;
				
				for(int i = first_group.first; i < first_group.second; i++){
					for(int j = second_group.first; j < second_group.second; j++){
						trees->push_back(TreeNode(trees.get(), i, j));
					}
				}
			}
		}
		groups[current_nodes] = {start, trees->size()};
	}
		
	return move(trees);
}

TreeSizes::TreeSizes(unique_ptr<vector<TreeNode>>&& trees) :
	trees(move(trees)),
	sizes(tree_less) {}

const vector<TreeNode>& TreeSizes::get_trees() const{
	return *trees;
}
const vector<unsigned int>& TreeSizes::get_sizes(const TreeNode& node) const{
	return sizes.at(node);
}
void TreeSizes::put_sizes(const TreeNode& node, vector<unsigned int>&& node_sizes){
	sizes[node] = move(node_sizes);
}

TreeSizes get_tree_sizes(const graph_t& graph, const vector<unsigned int>& nodes){
	TreeSizes sizes(get_trees(nodes));
		
	for(auto tree: sizes.get_trees()){
		map<unsigned int, unsigned int> starting_points;
		if(tree.is_leaf()){
			starting_points[tree.get_value()] = 0;
		}
		else{
			for(int i = 0; i < graph.size(); i++){
				starting_points[i] = sizes.get_sizes(tree.get_left())[i] + sizes.get_sizes(tree.get_right())[i];
			}
		}
		sizes.put_sizes(tree, get_dist_map(graph, starting_points));
	}
	
	return move(sizes);
}

vector<unsigned int> get_path(const graph_t& graph, const vector<unsigned int>& distance_map, unsigned start){
	vector<unsigned int> path({start});
	
	while(true){
		unsigned int next = path.back();
		unsigned int dist = distance_map[next];
		
		for(auto neighbor: graph[path.back()]){
			int neighbor_distance = distance_map[neighbor.first] + neighbor.second;
			if(neighbor_distance <= dist){
				next = neighbor.first;
				dist = neighbor_distance;
			}
		}
		
		if(next == path.back()) return move(path);
		path.push_back(next);
	}
}

TreeNode get_minimal_tree(const TreeSizes& sizes, unsigned int extra_node){
	TreeNode minimal = sizes.get_trees()[0];
	int leaves_num = 0;
	for(TreeNode current: sizes.get_trees()){
		int cur_leaves_num = current.get_leaves().size();
		if(cur_leaves_num > leaves_num){
			leaves_num = cur_leaves_num;
			minimal = current;
		}
		else if(cur_leaves_num < leaves_num) continue;

		if(sizes.get_sizes(current)[extra_node] < sizes.get_sizes(minimal)[extra_node]){
			minimal = current;
		}
	}
	return minimal;
}

unsigned int min_connection_size(const graph_t& graph, vector<unsigned int> nodes){
	unsigned int special_node = nodes.back();
	nodes.pop_back();
	if(!nodes.size()) return 0;
	TreeSizes sizes(get_tree_sizes(graph, nodes));
	TreeNode tree(get_minimal_tree(sizes, special_node));

	return sizes.get_sizes(tree)[special_node];
}

set<pair<unsigned int, unsigned int>> min_connection_tree(const graph_t& graph, vector<unsigned int> nodes){
	unsigned int special_node = nodes.back();
	nodes.pop_back();
	set<pair<unsigned int, unsigned int>> tree_edges;

	if(nodes.size()) {
		TreeSizes sizes(get_tree_sizes(graph, nodes));
		TreeNode tree(get_minimal_tree(sizes, special_node));
		
		vector<pair<TreeNode, unsigned int>> stack({{tree, special_node}});
		
		while(stack.size()){
			auto current = stack.back();
			stack.pop_back();
			
			vector<unsigned int> path = get_path(graph, sizes.get_sizes(current.first), current.second);
			
			for(int i = 1; i < path.size(); i++){
				tree_edges.insert(minmax(path[i], path[i - 1]));
			}
						
			if(!current.first.is_leaf()){
				stack.push_back({current.first.get_left(), path.back()});
				stack.push_back({current.first.get_right(), path.back()});
			}
		}
	}
	
	return move(tree_edges);
}