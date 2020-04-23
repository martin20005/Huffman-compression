#include "node.h"

// Class Node
Node::Node() :
    left_child_(nullptr),
    right_child_(nullptr),
    weight_(0) { }
Node::Node(Node *left, Node *right, long weight) :
    left_child_(left),
    right_child_(right),
    weight_(weight) {}

Node *&Node::left()               { return left_child_; }
void Node::left(Node *l)          { left_child_ = l; }
Node *&Node::right()              { return right_child_; }
void Node::right(Node *r)         { right_child_ = r; }
long Node::weight() const          { return weight_; }
void Node::weight(long new_weight) { weight_ = new_weight; }

// Class End
End::End(Letter &letter, long frequency) :
    letter_(letter),
    Node(nullptr, nullptr, frequency) {}
End::End(const End &end) :
    letter_(end.letter_),
    Node(end.left(), end.right(), end.weight()) {}
End::End() :
    Node(), letter_(Letter()) {}

Letter &End::letter() { return letter_; }
Node *End::left() const { return nullptr; }
Node *End::right() const { return nullptr; }

bool End::operator==(End &end) { return end.letter() == letter_; }
bool End::operator==(char ch)  { return letter_.original() == ch; }
void End::operator++(int)      { weight_++; }

// Class Path
Path::Path(Node *left, Node *right) {
    if (left == nullptr || right == nullptr) weight_ = 0;
    else weight_ = left->weight() + right->weight();
    left_child_ = left;
    right_child_ = right;
}
