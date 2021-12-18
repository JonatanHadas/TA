#ifndef _SUBSET_H
#define _SUBSET_H

#include <vector>

using namespace std;

class SubsetMask;
class SubsetGroup;

class SubsetMask{
    const vector<bool>* containing_mask;
    vector<bool> mask;

    SubsetMask(const vector<bool>& mask, const vector<bool>* containing_mask);
	friend SubsetGroup;
public:
    SubsetMask(unsigned int size);
    SubsetMask(const vector<bool>& mask);
    SubsetMask(const SubsetGroup& superset);

    vector<unsigned int> get_indices() const;

    const vector<bool>& operator*() const;
    bool operator==(const SubsetMask& other) const;
    bool operator!=(const SubsetMask& other) const;

    const SubsetMask& operator++();
};

class SubsetGroup{
    const vector<bool> mask;

    friend SubsetMask;
public:
    SubsetGroup(unsigned int size);
    SubsetGroup(const vector<bool> mask);

    SubsetMask begin() const;
    SubsetMask end() const;
	
	vector<bool> inverse(const vector<bool>& subset_mask);
};

#endif