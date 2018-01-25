#include<iostream>

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

    static void merge_sort(int sz, iterator left, iterator right, List& buffer);
    static void merge(iterator left, iterator mid, iterator right, List& buffer);
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
    List buffer;
    for(int i = 0; i < size_; ++i) {
        buffer.push_back(0);
    }
    merge_sort(size_, this->begin(), this->end(), buffer);
}

// MergeSort

void List::merge_sort(int sz, ListIter left, ListIter right, List& buffer) {
    if(sz == 1) {
        return;
    }
    auto mid = left;
    for(int i = 0; i < sz / 2; ++i) {
        ++mid; 
    }
    merge_sort(sz / 2, left, mid, buffer);
    merge_sort(sz - sz / 2, mid, right, buffer);
    merge(left, mid, right, buffer);
}

void List::merge(ListIter left, ListIter mid, ListIter right, List& buffer) {
    auto l = left, r = mid;
    auto it = buffer.begin();
    while(l != mid && r != right) {
        if(*l < *r) {
            *it++ = *l++;
        } else {
            *it++ = *r++;
        }
    }
    while(l != mid) {
        *it++ = *l++;
    }
    while(r != right) {
        *it++ = *r++;
    }
    it = buffer.begin();
    while(left != right) {
        *left++ = *it++;
    }
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
