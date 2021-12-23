#ifndef _DISJOINT_SETS_H
#define _DISJOINT_SETS_H

#include <vector>

using namespace std;

class DisjointSets{
    mutable vector<unsigned int> parents;
    vector<unsigned int> ranks;

    
public:
    DisjointSets(unsigned int size);

    unsigned int get_set(unsigned int member) const;
    void unify(unsigned int member1, unsigned int member2);
};

#endif