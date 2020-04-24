#ifndef HUFFMAN_BITBUFFER_H
#define HUFFMAN_BITBUFFER_H

#include <iostream>
#include "list.h"
/**@class BitBuffer
 * This class can hold bits. Useful when size of values to be stored is not constant. */
class BitBuffer {
    bool is_open_;              ///< Editing is allowed (close() not called)
    List<char> data_;           ///< All the chunks (if 'is_open_', it doesn't contain 'current_chunk_')
    char current_chunk_;        ///< The last chunk (maybe not full)
    int len_of_curr_chunk_b_;   ///< Count of pushed bits in 'current_chunk_'
    int size_of_chunk_b_;       ///< size of a single chunk
    int max_chunks_;            ///< If set, calling leak() empties some chunks (while weight_() >= max_chunks_)
    int first_unpopped_bit_;    ///< Shows which is the first not-yet-popped bit (calling bit() alters it)

public:
    /**Buffer
     * @param size_of_chunk_b  Number of bits each chunk can store (character's size by default) */
    BitBuffer(int size_of_chunk_b = 8*sizeof(char));
    /**Buffer
     * @param buffer  A buffer to be copied (resulting Buffer is editable) */
    BitBuffer(const BitBuffer& buffer);

    /**@return data_ stored so far */
    const List<char>& data() const { return data_; }
    /**@return Last (not yet full) chunk of data_. The last count() bits are values. */
    char currentChunk() const { return current_chunk_; }
    /**@return Count of valuable bits in currentChunk() */
    int count() const { return len_of_curr_chunk_b_; }
    /**@return Count of bits in each chunk. */
    int sizeOfChunk() const { return size_of_chunk_b_; }
    /**@return Number of full chunks. */
    int countOfFullChunks() const { return data_.count(); }
    /**@return true, if there is no pop()-able data_ in the buffer */
    bool isEmpty() { return data_.count() <= 0; }
    /**@return Whether buffer is open (bits can be push()-ed) */
    bool isOpen() { return is_open_; }

    /**Adding bits to the buffer
     * Pushes the given bits into the buffer (current_chunk_).
     * @param value_bit_container  It contains the value-bits (fitted to the right_child_=LSB).
     * @param count_of_bits  Number of significant bits.*/
    template<typename T>
    void push(T value_bit_container, int count_of_bits) {
        if (!is_open_) return;                          // if editing not allowed
        while (count_of_bits > 0) {
            if (len_of_curr_chunk_b_ == sizeOfChunk()) {// if 'current_chunk_' is full, add it to 'data_'
                len_of_curr_chunk_b_ = 0;
                data_.add(current_chunk_);
                current_chunk_ = 0;
            }
            current_chunk_ <<= 1;                       // making place for a new bit
            current_chunk_ |= ((value_bit_container >> --count_of_bits) & 1); // adding the new bit
            len_of_curr_chunk_b_++;
        }
    }

    /**Returning the first chunk
     * The returned chunk will be removed from buffer.
     * @return The first chunk*/
    char pop();

    /**Closing the buffer
     * This makes sure that all data_ is stored properly;
     * using push() will not be allowed anymore.*/
    void close();

    /**Setting the leak-size*/
    void leakAfter(int num_of_chunks);
    /**Get leak-size */
    int leakAfter() const;
    /**Leaking data_ to a file
     * When there'are more chunks than the leak-size,
     * those all go to the file*/
    void leak(std::ostream& stream_out);
    /**Getting some data_ from file
     * After call, at least 1 full character will be stream_in the buffer. */
    void fill(std::istream& stream_in);
    /**Returning the first (not yet returned) bit from buffer
     * @return True = 1, False = 0 bit */
    bool bit();
};


#endif //HUFFMAN_BITBUFFER_H
