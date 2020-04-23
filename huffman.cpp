#include "memtrace.h"
#include "huffman.h"
#include "node.h"

Huffman::Huffman() : root_h_tree_(nullptr) { }
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
// ----------------------------- Compression ------------------------------- //
void Huffman::compress(istream &stream_in, ostream &stream_out) {
    List<End> freq_of_inputs;
    char input;

    while (!stream_in.eof()) {                                     // until end_eof of file:
        stream_in.get(input);                                   // read an input (char)
        Letter tmp_letter = Letter(input);
        End tmp_end(tmp_letter, 1);
        int idx_of_input = freq_of_inputs.findFirst(tmp_end);
        if (idx_of_input >= 0) { freq_of_inputs[idx_of_input]++; } // if it already existed, just increment frequency
        else { freq_of_inputs.add(tmp_end); }                   // else add it to the "dictionary"
    }
    Letter letter_eof = Letter(EOF);
    End end_eof = (End(letter_eof, 1));                // EOF (as a signal of compressed data_'s end)
    freq_of_inputs.add(end_eof);

    root_h_tree_ = genTree(freq_of_inputs);                     // generate the H-tree
    genCodes(root_h_tree_);                                        // setting H-codes by H-tree

    stream_out << (unsigned char) freq_of_inputs.count();                       // Meta 0: dictionary's size
    for (int i = 0; i < freq_of_inputs.count(); ++i) {
        stream_out << freq_of_inputs[i].letter().original();                    // Meta N 0: original character
        stream_out << freq_of_inputs[i].letter().length();                      // Meta N 2: valuable bits in H-code
        long long int huffman = freq_of_inputs[i].letter().huffman();
        /* bytes needed to store H-code: */
        int cnt_B = (freq_of_inputs[i].letter().length() + 7) / (8 * sizeof(char));
        for (int j = cnt_B; j > 0; j--) {                                       // Meta N 1: huffman-code
            /*this breaks the huffman-codes to 8bit pieces*/
            stream_out << (char) (255 & (huffman >> ((j - 1) * 8)));
        }
    }

    stream_in.clear();                                              // Clearing flags, so input can be read again
    stream_in.seekg(0, stream_in.beg);
    BitBuffer buffer = BitBuffer();
    buffer.leakAfter(50);                              // After every 50th byte, data_ leaks to stream_out
    while (!stream_in.eof()) {
        stream_in.get(input);                                                // read
        Letter l = Letter(input);                                               // converting char to End
        End e(l, 0);
        int idx = freq_of_inputs.findFirst(e);                               // getting its H-code
        l = freq_of_inputs[idx].letter();
        buffer.push(l.huffman(), l.length());                                   // pushing H-code to buffer
        buffer.leak(stream_out);                                             // after 50 bytes, write
    }

    letter_eof = Letter(EOF);                                       // Pushing an EOF
    end_eof = End(letter_eof, 0);
    int idx_eof = freq_of_inputs.findFirst(end_eof);
    buffer.push(freq_of_inputs[idx_eof].letter().huffman(), freq_of_inputs[idx_eof].letter().length());
    buffer.close(); //note: this must be closed

    while (!buffer.isEmpty())  { stream_out << buffer.pop(); }      // Writing the not-yet-leaked (compressed) data_
    delRoot(root_h_tree_);                                          // Deleting the H-tree
}
#pragma clang diagnostic pop




Node *Huffman::genTree(List<End> &leaves) {
    List<Node*> nodes = List<Node*>();
    for (int i = 0; i < leaves.count(); ++i) {                      // Copying so the original list won't be modified
        Node* n = &leaves[i];
        nodes.add(n);
    }
    while (nodes.count() > 1) {
        Node* new_node;
        int min_idx = 0;
        Node* min_node_0;
        Node* min_node_1;
        for (int i = 1; i < nodes.count(); ++i)                     // searching minimum
            if (nodes[i]->weight() < nodes[min_idx]->weight())
                min_idx = i;
        min_node_0 = nodes[min_idx];
        nodes.removeAt(min_idx);
        min_idx = 0;
        for (int i = 1; i < nodes.count(); ++i)                     // searching second minimum
            if (nodes[i]->weight() < nodes[min_idx]->weight() )
                min_idx = i;
        min_node_1 = nodes[min_idx];
        nodes.removeAt(min_idx);

        if (nodes.count() == 0) return new Path(min_node_0, min_node_1);
        new_node = new Path(min_node_0, min_node_1);                // merging the 2 nodes
        nodes.add(new_node);
    }
    return nodes[0];
}



void Huffman::genCodes(Node *root_h_tree, unsigned char hlen_b, long long int hcode) {
    if (root_h_tree->left() == nullptr /*&& root_h_tree->right() == nullptr*/) {    // if root_h_tree is an end
        ((End*)root_h_tree)->letter().huffman(hcode, hlen_b);                       // set the pre-built code
    }
    else {
        genCodes(root_h_tree->left(), hlen_b + 1, (hcode << 1) | 0);   // go on to the left_child_
        genCodes(root_h_tree->right(), hlen_b + 1, (hcode << 1) | 1);  // go on to the right_child_
    }
}



// ----------------------------- Extraction ----------------------------- //
void Huffman::extract(istream& stream_in, ostream& stream_out) {
    char input;
    stream_in.get(input);
    int cnt_of_ends = (int) input;                                          // Getting size of dictionary

    List<End> ends;
    char hcode, hlen_b;

    while (cnt_of_ends-- > 0) {
        stream_in.get(input);                                               // Original character
        stream_in.get(hlen_b);                                              // Length of H-code (in bits)
        long long int full_hcode = 0;
        int rel_size = (hlen_b + 7) / (8*sizeof(char));
        for (int which_byte = rel_size; which_byte > 0; which_byte--) {        // Getting H-code
            /*Getting bytes one by one*/
            stream_in.get(hcode);
            full_hcode <<= 8*sizeof(char);
            full_hcode |= (unsigned char) hcode;
        }
        Letter tmp_letter(input);                                              // Adding data_ to list
        tmp_letter.huffman(full_hcode, (unsigned char) hlen_b);
        End tmp_end = End(tmp_letter, 0);
        ends.add(tmp_end);
    }

    root_h_tree_ = new Path(nullptr, nullptr);
    for (int e_idx = 0; e_idx < ends.count(); e_idx++)                     // Building the H-tree
        rebuildTree(root_h_tree_, &ends[e_idx], ends[e_idx].letter().length());

    BitBuffer buffer = BitBuffer();
    buffer.fill(stream_in);                                             // Pushing some inputs to the buffer
    Node* curr_node;
    char output = ' ';/* Anything not EOF */

    while (output != EOF) {
        curr_node = root_h_tree_;                                               // Starting at H-tree's root
        buffer.fill(stream_in);                                              // Maybe buffer is empty
        while (curr_node->right() != nullptr)                                   // While not hitting a character,
            curr_node = buffer.bit() ? curr_node->right() : curr_node->left();  // move on
        output = ((End*)curr_node)->letter().original();                        // Set output
        if (output != EOF) stream_out << output;                                // if it's not EOF, "write"
    }
    delRoot(root_h_tree_);                                                  // Deleting H-tree
}



void Huffman::rebuildTree(Node *&root_h_tree, End *curr_leaf, int bit_in_hcode) {
    Node* next;
    if (bit_in_hcode == 1) { next = curr_leaf; }                                // if next node is an End
    else { next = new Path(nullptr, nullptr); }                            // else it's a Path

    if ((curr_leaf->letter().huffman() >> (bit_in_hcode - 1) & 1) == 1) {       // if it's a 1 (right_child_ turn)
        if (root_h_tree->right() == nullptr) root_h_tree->right(next);               // if it's not yet visited
        else delete next;                                                            // else 'next' is useless
        if (bit_in_hcode == 1) return;                                               // if route finished, return
        rebuildTree(root_h_tree->right(), curr_leaf, bit_in_hcode - 1);// else go on
    }
    else {                                                                      // else it's a 0 (left_child_ turn)
        if (root_h_tree->left() == nullptr) root_h_tree->left(next);                 // not yet visited
        else delete next;                                                            // already visited
        if (bit_in_hcode == 1) return;                                               // route finished
        rebuildTree(root_h_tree->left(), curr_leaf, bit_in_hcode - 1); // else go on
    }
}

void Huffman::delRoot(Node *root_of_tree) {
    /* Delete with DFS */
    if (root_of_tree->left() != nullptr) delRoot(root_of_tree->left());
    if (root_of_tree->right() != nullptr) delRoot(root_of_tree->right());
    if (nullptr != root_of_tree->left()) delete root_of_tree;
}

Huffman::~Huffman() {}

