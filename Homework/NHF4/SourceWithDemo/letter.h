#ifndef HUFFMAN_LETTER_H
#define HUFFMAN_LETTER_H

/**@class Letter
 * Bounds original character and Huffman-code together*/
class Letter {
    char original_;          ///< The original character
    long long int huffman_code_; ///< The Huffman-code
    unsigned char len_of_code_b_;  ///< Length of H-code
public:
    /**Constructor
     * Only the original character is set*/
    explicit Letter(char ch) :
        original_(ch),
        huffman_code_(0),
        len_of_code_b_(0) { }
    /**Copy constructor */
    Letter(const Letter& letter) :
        original_(letter.original()),
        huffman_code_(letter.huffman()),
        len_of_code_b_(letter.length()) {}
    /**isEmpty Letter for creating arrays */
    Letter() :
        original_(0),
        huffman_code_(0),
        len_of_code_b_(0) { }

    /**@return The original character */
    char original() const { return original_; }
    /**Setting the original character */
    void original(char ch) { original_ = ch; }
    /**@return The Huffman-code of character */
    long long int huffman() const { return huffman_code_; }
    /**Setting the Huffman-code (length of H-code always needed!) */
    void huffman(long long int h, unsigned char l) { huffman_code_ = h; len_of_code_b_ = l; }
    /**@return The length of Huffman-code */
    unsigned char length() const { return len_of_code_b_; }

    /**Assignment */
    Letter& operator=(const Letter& letter) {
        if (&letter == this) return *this;
        original_ = letter.original();
        huffman_code_ = letter.huffman();
        len_of_code_b_ = letter.length();
        return *this;
    }
    /**Equal if the original characters equal */
    bool operator==(const Letter& letter) const {
        return letter.original() == original_;
    }
};

#endif //HUFFMAN_LETTER_H
