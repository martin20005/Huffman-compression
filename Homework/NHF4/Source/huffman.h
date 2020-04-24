#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <utility>
#include "list.h"
#include "node.h"
#include "bitbuffer.h"
using namespace std;

/**@class Huffman
 * Class able to execute Huffman-compression or extraction*/
class Huffman {
    Node* root_h_tree_; ///< Root of Huffman-tree
public:
    Huffman();
    // ---------------------------- Compression ---------------------------- //
public:
    /**Compression of file */
    void compress(istream& stream_in, ostream& stream_out);

private:
    /**Generating the Huffman-tree
     * @param leaves  List of end-nodes (which contain Letters and frequencies)
     * @pre Count frequencies of letters in file, create a List<End>*/
    static Node* genTree(List<End>& leaves);
    /**Generating the Huffman-codes by H-tree
     * @param root_h_tree   Root of Huffman-tree
     * @param hlen_b        Length of Huffman-code (count of bits)
     * @param hcode         The Huffman-code
     * @pre Call genTree */
    static void genCodes(Node* root_h_tree, unsigned char hlen_b = 0, long long int hcode = 0);

    // ---------------------------- Extraction ---------------------------- //
public:
    /**Extraction of .huffman_code_ file */
    void extract(istream &stream_in, ostream &stream_out);

private:
    /**Rebuilding the H-tree
     * This rebuilds the whole H-tree if all the leaves are correct,
     * must be called for each leaf.
     * @pre Read metadata from file, create a List<end> */
    static void rebuildTree(Node*& root_h_tree, End* curr_leaf, int bit_in_hcode);

    // --------------------------- General functions --------------------------- //
private:
    /**Deleting binary tree (Huffman-tree)
     * @param root_of_tree  Root of tree*/
    void delRoot(Node* root_of_tree);
public:
    ~Huffman();
};


#endif //HUFFMAN_HUFFMAN_H
