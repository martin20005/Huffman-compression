#ifndef LIST_H
#define LIST_H
#include <cstddef>

template <class T>
class List {
    T* data_;      ///< Array of data (of type T)
    int len_;      ///< Count of used data
    int cap_;      ///< Capacity of the list (auto-incremented)
    int inc_size_; ///< Size of incrementation when running out of space
public:
    // Constructors:

    explicit List() : data_(new T[10]), len_(0), cap_(10), inc_size_(10) {}
    List(const List& list) {
        if (list.count() == 0)
            List();
        else {
            len_ = list.count();
            inc_size_ = 10;
            cap_ = len_;
            data_ = new T[cap_];
            for (int i = 0; i < len_; i++) { data_[i] = list[i]; }
        }
    }
    ~List() { delete[] data_; }

    // Common functions:
    int count() const { return len_; }
    void add(T element) {
        if (len_ >= cap_) {
            cap_ += inc_size_;
            T* new_data = new T[cap_];
            for (int i = 0; i < len_; i++) { new_data[i] = data_[i]; }
            delete[] data_;
            data_ = new_data;
        }
        data_[len_++] = element;
    }
    void removeAt(int idx) {
        if (idx < 0 || idx >= len_) return;
        for (int i = idx; i < len_ - 1; i++) { data_[i] = data_[i + 1]; }
        len_--;
    }
    void remove(T& element) {
        int where = findFirst(element);
        if (where != -1) removeAt(where);
    }
    int findFirst(T& element) {
        for (int i = 0; i < len_; ++i) {
            if (element == data_[i]) return i;
        }
        return -1;
    }

    // Operators:

    /** @return Element with index 'idx' */
    T& operator[](int idx) const {
        if (idx < 0 || idx >= len_) throw 1;
        return data_[idx];
    }

    /** Addition of element with operator */
    List& operator+=(T element) {
        this->add(element);
        return *this;
    }
};

#endif //LIST_H
