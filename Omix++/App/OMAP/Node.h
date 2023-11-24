/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Node.h
 * Author: jgc
 *
 * Created on April 19, 2021, 11:36 AM
 */

#ifndef NODE_H
#define NODE_H

#include "AES.hpp"
#include "Bid.h"

class Node {
public:

    Node() {
    }

    ~Node() {
    }
    unsigned long long index;
    std::array< byte_t, 16> value;
    Bid key;
    unsigned long long pos;
    int height;
    long long evictionNode;
    bool isDummy;
    Bid leftID;
    Bid rightID;
    bool modified;
    unsigned long long leftPos;
    unsigned long long rightPos;
    std::array< byte_t, 24> dum;

    static Node* clone(Node* oldNode) {
        Node* newNode = new Node();
        newNode->evictionNode = oldNode->evictionNode;
        newNode->index = oldNode->index;
        newNode->key = oldNode->key;
        newNode->pos = oldNode->pos;
        newNode->rightID = oldNode->rightID;
        newNode->leftID = oldNode->leftID;
        newNode->value = oldNode->value;
        newNode->isDummy = oldNode->isDummy;
        newNode->modified = oldNode->modified;
        newNode->leftPos = oldNode->leftPos;
        newNode->height = oldNode->height;
        newNode->rightPos = oldNode->rightPos;
        newNode->dum = oldNode->dum;
        return newNode;
    }

    /**
     * constant time comparator
     * @param left
     * @param right
     * @return left < right -> -1,  left = right -> 0, left > right -> 1
     */
    static int CTcmp(long long lhs, long long rhs) {
        unsigned __int128 overflowing_iff_lt = (__int128) lhs - (__int128) rhs;
        unsigned __int128 overflowing_iff_gt = (__int128) rhs - (__int128) lhs;
        int is_less_than = (int) -(overflowing_iff_lt >> 127); // -1 if self < other, 0 otherwise.
        int is_greater_than = (int) (overflowing_iff_gt >> 127); // 1 if self > other, 0 otherwise.
        int result = is_less_than + is_greater_than;
        return result;
    }

    /**
     * constant time selector
     * @param a
     * @param b
     * @param choice 0 or 1
     * @return choice = 1 -> a , choice = 0 -> return b
     */
    static long long conditional_select(long long a, long long b, int choice) {
        unsigned long long one = 1;
        return (~((unsigned long long) choice - one) & a) | ((unsigned long long) (choice - one) & b);
    }

    static unsigned long long conditional_select(unsigned long long a, unsigned long long b, int choice) {
        unsigned long long one = 1;
        return (~((unsigned long long) choice - one) & a) | ((unsigned long long) (choice - one) & b);
    }

    static unsigned int conditional_select(unsigned int a, unsigned int b, int choice) {
        unsigned int one = 1;
        return (~((unsigned int) choice - one) & a) | ((unsigned int) (choice - one) & b);
    }

    /**
     * constant time selector
     * @param a
     * @param b
     * @param choice 0 or 1
     * @return choice = 1 -> a , choice = 0 -> return b
     */
    static unsigned __int128 conditional_select(unsigned __int128 a, unsigned __int128 b, int choice) {
        unsigned __int128 one = 1;
        return (~((unsigned __int128) choice - one) & a) | ((unsigned __int128) (choice - one) & b);
    }

    /**
     * constant time selector
     * @param a
     * @param b
     * @param choice 0 or 1
     * @return choice = 1 -> a , choice = 0 -> return b
     */
    static byte_t conditional_select(byte_t a, byte_t b, int choice) {
        byte_t one = 1;
        return (~((byte_t) choice - one) & a) | ((byte_t) (choice - one) & b);
    }

    /**
     * constant time selector
     * @param a
     * @param b
     * @param choice 0 or 1
     * @return choice = 1 -> b->a , choice = 0 -> return a->a
     */
    static void conditional_assign(Node* a, Node* b, int choice) {
        a->index = Node::conditional_select((long long) b->index, (long long) a->index, choice);
        a->isDummy = Node::conditional_select(b->isDummy, a->isDummy, choice);
        a->pos = Node::conditional_select((long long) b->pos, (long long) a->pos, choice);
        for (int k = 0; k < b->value.size(); k++) {
            a->value[k] = Node::conditional_select(b->value[k], a->value[k], choice);
        }
        for (int k = 0; k < b->dum.size(); k++) {
            a->dum[k] = Node::conditional_select(b->dum[k], a->dum[k], choice);
        }
        a->evictionNode = Node::conditional_select(b->evictionNode, a->evictionNode, choice);
        a->modified = Node::conditional_select(b->modified, a->modified, choice);
        a->height = Node::conditional_select(b->height, a->height, choice);
        a->leftPos = Node::conditional_select(b->leftPos, a->leftPos, choice);
        a->rightPos = Node::conditional_select(b->rightPos, a->rightPos, choice);
        for (int k = 0; k < a->key.id.size(); k++) {
            a->key.id[k] = Node::conditional_select(b->key.id[k], a->key.id[k], choice);
        }
        for (int k = 0; k < a->leftID.id.size(); k++) {
            a->leftID.id[k] = Node::conditional_select(b->leftID.id[k], a->leftID.id[k], choice);
        }
        for (int k = 0; k < a->rightID.id.size(); k++) {
            a->rightID.id[k] = Node::conditional_select(b->rightID.id[k], a->rightID.id[k], choice);
        }
    }

    /**
     * constant time selector
     * @param a
     * @param b
     * @param choice 0 or 1
     * @return choice = 1 -> a , choice = 0 -> return b
     */
    static void conditional_swap(Node* a, Node* b, int choice) {
        Node tmp = *b;
        b->index = Node::conditional_select((long long) a->index, (long long) b->index, choice);
        b->isDummy = Node::conditional_select(a->isDummy, b->isDummy, choice);
        b->pos = Node::conditional_select((long long) a->pos, (long long) b->pos, choice);
        for (int k = 0; k < b->value.size(); k++) {
            b->value[k] = Node::conditional_select(a->value[k], b->value[k], choice);
        }
        for (int k = 0; k < b->dum.size(); k++) {
            b->dum[k] = Node::conditional_select(a->dum[k], b->dum[k], choice);
        }
        b->evictionNode = Node::conditional_select(a->evictionNode, b->evictionNode, choice);
        b->modified = Node::conditional_select(a->modified, b->modified, choice);
        b->height = Node::conditional_select(a->height, b->height, choice);
        b->leftPos = Node::conditional_select(a->leftPos, b->leftPos, choice);
        b->rightPos = Node::conditional_select(a->rightPos, b->rightPos, choice);
        for (int k = 0; k < b->key.id.size(); k++) {
            b->key.id[k] = Node::conditional_select(a->key.id[k], b->key.id[k], choice);
        }
        for (int k = 0; k < b->leftID.id.size(); k++) {
            b->leftID.id[k] = Node::conditional_select(a->leftID.id[k], b->leftID.id[k], choice);
        }
        for (int k = 0; k < b->rightID.id.size(); k++) {
            b->rightID.id[k] = Node::conditional_select(a->rightID.id[k], b->rightID.id[k], choice);
        }

        a->index = Node::conditional_select((long long) tmp.index, (long long) a->index, choice);
        a->isDummy = Node::conditional_select(tmp.isDummy, a->isDummy, choice);
        a->pos = Node::conditional_select((long long) tmp.pos, (long long) a->pos, choice);
        for (int k = 0; k < b->value.size(); k++) {
            a->value[k] = Node::conditional_select(tmp.value[k], a->value[k], choice);
        }
        for (int k = 0; k < b->dum.size(); k++) {
            a->dum[k] = Node::conditional_select(tmp.dum[k], a->dum[k], choice);
        }
        a->evictionNode = Node::conditional_select(tmp.evictionNode, a->evictionNode, choice);
        a->modified = Node::conditional_select(tmp.modified, a->modified, choice);
        a->height = Node::conditional_select(tmp.height, a->height, choice);
        a->leftPos = Node::conditional_select(tmp.leftPos, a->leftPos, choice);
        a->rightPos = Node::conditional_select(tmp.rightPos, a->rightPos, choice);
        for (int k = 0; k < a->key.id.size(); k++) {
            a->key.id[k] = Node::conditional_select(tmp.key.id[k], a->key.id[k], choice);
        }
        for (int k = 0; k < a->leftID.id.size(); k++) {
            a->leftID.id[k] = Node::conditional_select(tmp.leftID.id[k], a->leftID.id[k], choice);
        }
        for (int k = 0; k < a->rightID.id.size(); k++) {
            a->rightID.id[k] = Node::conditional_select(tmp.rightID.id[k], a->rightID.id[k], choice);
        }
    }

    static void conditional_swap(unsigned long long& a, unsigned long long& b, int choice) {
        unsigned long long tmp = b;
        b = Node::conditional_select((long long) a, (long long) b, choice);
        a = Node::conditional_select((long long) tmp, (long long) a, choice);
    }

    /**
     * constant time selector
     * @param a
     * @param b
     * @param choice 0 or 1
     * @return choice = 1 -> a , choice = 0 -> return b
     */
    static int conditional_select(int a, int b, int choice) {
        unsigned int one = 1;
        return (~((unsigned int) choice - one) & a) | ((unsigned int) (choice - one) & b);
    }

    static bool CTeq(int a, int b) {
        return !(a^b);
    }

    static bool CTeq(long long a, long long b) {
        return !(a^b);
    }

    static bool CTeq(unsigned __int128 a, unsigned __int128 b) {
        return !(a^b);
    }

    static bool CTeq(unsigned long long a, unsigned long long b) {
        return !(a^b);
    }


};

#endif /* NODE_H */

