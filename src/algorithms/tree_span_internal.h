#ifndef _TREE_SPAN_INTERNAL_H
#define _TREE_SPAN_INTERNAL_H

#include "tree_span.h"

#include <memory>

using namespace std;

vector<unsigned int> get_dist_map(const graph_t& graph, const map<unsigned int, unsigned int>& starting_points);

class TreeNode{
    const vector<TreeNode>* container;
    bool leaf;
    union{
        unsigned int value;
        struct{
            unsigned int left;
            unsigned int right;
        };
    };
public:
    TreeNode(const vector<TreeNode>* contianer, unsigned int value);
    TreeNode(const vector<TreeNode>* container, unsigned int left, unsigned int right);

    bool is_leaf() const;
    set<unsigned int> get_leaves() const;

    unsigned int get_value() const;
    const TreeNode& get_left() const;
    const TreeNode& get_right() const;
};

int tree_compare(const TreeNode& first, const TreeNode& second);

auto tree_less = [](const TreeNode& node1, const TreeNode& node2){return tree_compare(node1, node2) < 0;};

unique_ptr<vector<TreeNode>> get_trees(const vector<unsigned int>& nodes);

class TreeSizes{
	unique_ptr<vector<TreeNode>> trees;
	map<TreeNode, vector<unsigned int>, decltype(tree_less)> sizes;
public:
	TreeSizes(unique_ptr<vector<TreeNode>>&& trees);

	const vector<TreeNode>& get_trees() const;
	const vector<unsigned int>& get_sizes(const TreeNode& node) const;
	void put_sizes(const TreeNode& node, vector<unsigned int>&& node_sizes);
};

TreeSizes get_tree_sizes(const graph_t& graph, const vector<unsigned int>& nodes);

vector<unsigned int> get_path(const graph_t& graph, const vector<unsigned int>& distance_map, unsigned start);

TreeNode get_minimal_tree(const TreeSizes& sizes, unsigned int extra_node);

#endif

