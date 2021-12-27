#include "disjoint_sets.h"

DisjointSets::DisjointSets(unsigned int size) : 
    parents(size),
    ranks(size, 0)
    {
    for(unsigned int i = 0; i < size; i++){
        parents[i] = i;
    }
}

unsigned int DisjointSets::get_set(unsigned int member) const {
    unsigned int head = member;
    while(parents[head] != head) {
        head = parents[head];
    }
    while(member != head){
        unsigned int next = parents[member];
        parents[member] = head;
        member = next;
    }
    return head;
}

void DisjointSets::unify(unsigned int member1, unsigned int member2){
    member1 = get_set(member1);
    member2 = get_set(member2);

    if(member1 == member2) return;
    if(ranks[member1] < ranks[member2]){
        parents[member1] = member2;
    }
    else{
        if(ranks[member1] == ranks[member2]) ranks[member1]++;
        parents[member2] = member1;
    }
}
