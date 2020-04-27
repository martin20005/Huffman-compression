#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include "letter.h"
#include <cstdlib>

/**@class Node
 * Represents a Node of a binary (Huffman) tree*/
class Node {
protected:
    Node* left_child_; ///< Pointer to left_child_ child
    Node* right_child_;///< Pointer to right_child_ child
    long weight_;   ///< Weight of node
public:
    /**Constructor
     * Basically an empty node (useful for creating a list of 'Node'-s)*/
    Node();
    /**Constructor
     * Making a parent for 2 children nodes*/
    Node(Node* left, Node* right, long weight = 0);
    /**Returns left_child_ child's pointer's reference */
    Node*& left();
    /**Setter for the left_child_ child
     * @return The new left_child_'s pointer*/
    void left(Node* l);
    /**Returns right_child_ child's pointer's reference */
    Node*& right();
    /**Setter for the right_child_ child
     * @return The new right_child_'s pointer*/
    void right(Node* r);
    /**Weight of node */
    long weight() const;
    /**Setting the weight of node */
    void weight(long new_weight);
};

/**@class End
 * Represent a Node of a binary (Huffman) tree, which IS a leaf*/
class End :public Node {
    Letter letter_;    ///< Each leaf represents a letter of input
public:
    /**Constructor
     * @param letter  The represented Letter
     * @param frequency  Frequency of the letter in input*/
    End(Letter& letter, long frequency);
    /**Copy constructor */
    End(const End& end);
    /**isEmpty end-node; useful for arrays */
    End();
    /**Represented letter */
    Letter& letter();
    /**left_child_ child */
    Node* left() const;
    /**right_child_ child */
    Node* right() const;
    /**Equality operator (true if the same letter is represented) */
    bool operator==(End& end);
    /**True if End represents this character */
    bool operator==(char ch);
    /**Incrementing the frequency */
    void operator++(int);
};

/**@class Path
 * Represents a Node of a binary (Huffman) tree, which is NOT a leaf*/
class Path :public Node{
public:
    /**Merging 2 nodes
     * The new (parent) Node has a count equal to the sum of the children's count*/
    Path(Node* left, Node* right);
};

#endif //HUFFMAN_NODE_H
