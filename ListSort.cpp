#include <iostream>

class List {
public:
    List();
    ~List();
    void push_back(int x);
    size_t size() const;
    void sort();

    class iterator; 
    iterator begin() const;
    iterator end() const;

private:
    struct Node;
    Node *first, *last;
    int size_;

    static Node* merge_sort(int size, Node* left, Node* right);
    static Node* merge(Node* left, Node* right);
};

struct List::Node {
    Node *next;
    int val;
    
    Node(): next(nullptr) {};

    Node(Node *next_, int val_) : next(next_), val(val_) {}
};

List::List() : size_(0) {}

List::~List() {
    while(first != nullptr) {
        auto next = first->next;
        delete first;
        first = next;
    }
}

void List::push_back(int x) {
    Node *node = new Node(nullptr, x);
    if(size_ == 0) {
        first = last = node;
    } else {
        last->next = node;
        last = node;
    }
    ++size_;
}

size_t List::size() const {
    return size_;
}

class List::iterator {
public:
    iterator(Node *ptr_) : ptr(ptr_) {}

    iterator& operator ++() {
        ptr = ptr->next;
        return *this;
    }

    iterator operator ++(int) {
        auto tmp(*this);
        ++(*this);
        return tmp;
    }

    int& operator * () {
        return ptr->val;
    }

    bool operator == (const iterator& other) const {
        return ptr == other.ptr;
    }

    bool operator != (const iterator& other) const {
        return ptr != other.ptr;
    }

private:
    Node *ptr;
};

using ListIter = List::iterator;

ListIter List::begin() const {
    return ListIter(first);
} 

ListIter List::end() const {
    return ListIter(nullptr);
} 

void List::sort() {
    first = merge_sort(size_, first, last);
}

// MergeSort

List::Node* List::merge_sort(int size, Node* left, Node* right) {
    if(size == 1) {
        left->next = nullptr;
        return left;
    }
    auto mid = left;
    for(int i = 0; i < size / 2; ++i) {
        mid = mid->next;
    }
    return merge(merge_sort(size / 2, left, mid), merge_sort(size - size / 2, mid, right));
}

List::Node* List::merge(Node* left, Node* right) {
    Node *first, *last;
    if(right == nullptr || left->val < right->val) {
        first = last = left;
        left = left->next;
    } else {
        first = last = right;
        right = right->next;
    }
    while(left != nullptr && right != nullptr) {
        if(left->val < right->val) {
            last->next = left;
            last = left;
            left = left->next;
        } else {
            last->next = right;
            last = right;
            right = right->next;
        }
    }
    while(left != nullptr) {
        last->next = left;
        last = left;
        left = left->next;
    }
    while(right != nullptr) {
        last->next = right;
        last = right;
        right = right->next;
    }
    last->next = nullptr;
    return first;
}

int main() {
    int n;
    std::cin >> n;
    List list;
    for(int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        list.push_back(x);
    }
    list.sort();
    for(auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    return 0;
}
