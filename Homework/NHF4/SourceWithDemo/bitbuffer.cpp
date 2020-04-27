#include "bitbuffer.h"

BitBuffer::BitBuffer(int size_of_chunk_b):
        is_open_(true),
        data_(List<char>()),
        current_chunk_(0),
        len_of_curr_chunk_b_(0),
        size_of_chunk_b_(size_of_chunk_b),
        max_chunks_(50),
        first_unpopped_bit_(0) {}

BitBuffer::BitBuffer(const BitBuffer &buffer):
        is_open_(true),
        data_(List<char>(buffer.data())),
        current_chunk_(buffer.currentChunk()),
        len_of_curr_chunk_b_(buffer.count()),
        size_of_chunk_b_(buffer.sizeOfChunk()),
        max_chunks_(50),
        first_unpopped_bit_(0) {}

char BitBuffer::pop() {
    if (data_.count() < 1) return -1;
    char first = data_[0];
    data_.removeAt(0);
    return first;
}

void BitBuffer::close() {
    if (!is_open_) return;
    if (len_of_curr_chunk_b_ == 0) return;
    current_chunk_ <<= (size_of_chunk_b_ - len_of_curr_chunk_b_);
    data_.add(current_chunk_);
    is_open_ = false;
}

void BitBuffer::leak(std::ostream &stream_out) {
    if (data_.count() > max_chunks_) {
        while (data_.count() > 2) {
            stream_out << pop();
        }
    }
}

int BitBuffer::leakAfter() const { return max_chunks_; }

void BitBuffer::leakAfter(int num_of_chunks) { max_chunks_ = num_of_chunks; }

void BitBuffer::fill(std::istream &stream_in) {
    while (data_.count() < 3 && !stream_in.eof()) {
        char input;
        stream_in.get(input);
        push(input, 8*sizeof(char));
    }
    if (stream_in.eof()) close();
}

bool BitBuffer::bit() {
    if (size_of_chunk_b_ == first_unpopped_bit_) { pop(); first_unpopped_bit_ = 0; }
    if (data_.count() < 1) return false;
    return 1 == ((data_[0] >> (size_of_chunk_b_ - first_unpopped_bit_++ - 1)) & 1);
}

