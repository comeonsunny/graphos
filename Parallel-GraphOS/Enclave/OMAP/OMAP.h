#ifndef OMAP_H
#define OMAP_H
#include <iostream>
#include "ORAM.hpp"
#include <functional>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "AVLTree.h"
using namespace std;

class OMAP {
private:
    Bid rootKey;
    unsigned long long rootPos;



public:
    AVLTree* treeHandler;
    OMAP(int maxSize, bytes<Key> key, int instance);
    OMAP(int maxSize, bytes<Key> secretKey, map<Bid, string> pairs, map<unsigned long long, unsigned long long> permutation);
    virtual ~OMAP();
    void insert(Bid key, string value);
    string find(Bid key);
    void printTree();
    void batchInsert(map<Bid, string> pairs);
    vector<string> batchSearch(vector<Bid> keys);
    string setSpt(Bid key);
    string readAndSetDist(Bid key, string value);
    void setupInsert(map<Bid, string> pairs);
    string atomicReadAndSetDist(Bid key, string value);
    void atomicInsert(Bid key, string value);
    string atomicFind(Bid omapKey);
};

#endif /* OMAP_H */

