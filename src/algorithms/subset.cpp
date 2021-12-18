#include "subset.h"

SubsetMask::SubsetMask(const vector<bool>& mask, const vector<bool>* containing_mask) :
	mask(mask),
	containing_mask(containing_mask) {
}
SubsetMask::SubsetMask(unsigned int size) : SubsetMask(vector<bool>(size, false)) {}
SubsetMask::SubsetMask(const vector<bool>& mask) : SubsetMask(mask, NULL) {}
SubsetMask::SubsetMask(const SubsetGroup& superset) : SubsetMask(vector<bool>(superset.mask.size(), false), &superset.mask) {}

vector<unsigned int> SubsetMask::get_indices() const{
	vector<unsigned int> indices;
	for(int i = 0; i < mask.size(); i++){
		if(mask[i]) indices.push_back(i);
	}
	
	return move(indices);
}

const vector<bool>& SubsetMask::operator*() const{
	return mask;
}
bool SubsetMask::operator==(const SubsetMask& other) const{
	return mask == other.mask;
}
bool SubsetMask::operator!=(const SubsetMask& other) const{
	return mask != other.mask;
}

const SubsetMask& SubsetMask::operator++(){
	for(int i = 0; i < mask.size(); i++){
		if(NULL != containing_mask && !(*containing_mask)[i]) continue;
		mask[i] = !mask[i];
		if(mask[i]) break;
	}
	
	return *this;
}

SubsetGroup::SubsetGroup(unsigned int size) : SubsetGroup(vector<bool>(size, true)) {}
SubsetGroup::SubsetGroup(const vector<bool> mask) : mask(mask){
}

SubsetMask SubsetGroup::begin() const{
	SubsetMask it(*this);
	return ++it;
}
SubsetMask SubsetGroup::end() const{
	return SubsetMask(*this);
}

vector<bool> SubsetGroup::inverse(const vector<bool>& subset_mask){
	vector<bool> inverse_mask(mask.size());
	for(int i = 0; i < mask.size(); i++){
		inverse_mask[i] = mask[i] && !subset_mask[i];
	}
	
	return move(inverse_mask);
}
