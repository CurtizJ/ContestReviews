#include<iostream>

class List {
public:
    List();
    ~List();
    void push_back(int x);
    void pop_back();
    size_t size() const;
    void sort();

    class iterator; 
    iterator begin() const;
    iterator end() const;

private:
    struct Node;
    Node *first, *last;
    int size_;

    static void merge_sort(List& list);
    static void split(List& par, List& left, List& right, int mid);
    static void merge(List& list,  List& left,  List& right);
};

struct List::Node {
    Node *next;
    int val;
    
    Node(): next(nullptr) {};

    Node(Node *next_, int val_) : next(next_), val(val_) {}
};

List::List() : size_(0) {
    first = last = new Node();
}

List::~List() {
    while(first->next != nullptr) {
        auto tmp = first->next;
        delete first;
        first = tmp;
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

void List::sort() {
    merge_sort(*this);
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

// MergeSort

void List::merge_sort(List& list) {
    if(list.size() == 1) {
        return;
    }
    List left, right;
    split(list, left, right, list.size() / 2);
    merge_sort(left);
    merge_sort(right);
    return merge(list, left, right);
}

void List::split(List& par, List& left, List& right, int mid) {
    auto it = par.begin();
    for(int i = 0; i < mid; ++i) {
        left.push_back(*it++);
    }
    while(it != par.end()) {
        right.push_back(*it++);
    }
}

void List::merge(List& list, List& left, List& right) {
    auto l = left.begin(), r = right.begin();
    auto it = list.begin();
    while(l != left.end() && r != right.end()) {
        if(*l < *r) {
            *it++ = *l++;
        } else {
            *it++ = *r++;
        }
    }
    while(l != left.end()) {
        *it++ = *l++;
    }
    while(r != right.end()) {
        *it++ = *r++;
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
