#include <vector>
#include <list>
#include <stdexcept>
#include <initializer_list>

template<class K, class V, class Hash = std::hash<K>> 
class HashMap {
public:
    HashMap(Hash hasher_ = Hash{});
    
    template<class Iter>
    HashMap(Iter begin, Iter end, Hash hasher_ = Hash{});

    HashMap(std::initializer_list<std::pair<const K, V>> list, Hash _hasher = Hash{});

    HashMap(const HashMap& other);

    HashMap& operator = (const HashMap& other);

    size_t size() const;
    bool empty() const;
    Hash hash_function() const;

    using iterator = typename std::list<std::pair<const K, V>>::iterator;
    using const_iterator = typename std::list<std::pair<const K, V>>::const_iterator;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    iterator insert(const std::pair<const K, V>& value);
    iterator erase(const K& key);

    iterator find(const K& key);
    const_iterator find(const K& key) const;
    V& operator [] (const K& key);
    const V& at(const K& key) const;
    void clear();

private:
    std::list<std::pair<const K, V>> values;
    std::vector<std::pair<size_t, decltype(values.begin())>> table;
    Hash hasher;
    const double maxLoad = 0.8;
    const double minLoad = 0.2;
    const size_t minTableSize = 8;
    size_t size_ = 0, tableSize = minTableSize;
    
    size_t getIndex(const K& key) const;
    void rebuild(size_t newSize);
};

template<class K, class V, class Hash> 
HashMap<K, V, Hash>::HashMap(Hash hasher_) : hasher(hasher_) {
    table.resize(tableSize);
    for(size_t i = 0; i < tableSize; ++i) {
        table[i] = {0, values.begin()};
    }
}

template<class K, class V, class Hash> 
template<class Iter>
HashMap<K, V, Hash>::HashMap(Iter begin, Iter end, Hash hasher_) : hasher(hasher_) {
    table.resize(tableSize);
    for(size_t i = 0; i < tableSize; ++i) {
        table[i] = {0, values.begin()};
    }
    while(begin != end) {
        insert(*begin);
        ++begin;
    }
}

template<class K, class V, class Hash>
HashMap<K, V, Hash>::HashMap(std::initializer_list<std::pair<const K, V>> list, Hash hasher_) {
    *this = HashMap(list.begin(), list.end(), hasher_);
}

template<class K, class V, class Hash>
HashMap<K, V, Hash>::HashMap(const HashMap<K, V, Hash>& other) : table(other.table), hasher(other.hasher), tableSize(other.tableSize)  {
    size_ = other.size();
}

template<class K, class V, class Hash>
HashMap<K, V, Hash>& HashMap<K, V, Hash>::operator = (const HashMap<K, V, Hash>& other) {   
    size_ = 0;
    tableSize = other.tableSize;
    hasher = other.hasher;
    table.resize(tableSize);
    for(auto it = other.begin(); it != other.end(); ++it) {
        insert(*it);
    }
    return *this;
}

template<class K, class V, class Hash>
size_t HashMap<K, V, Hash>::getIndex(const K& key) const {
    return hasher(key) % tableSize;
}

template<class K, class V, class Hash>
void HashMap<K, V, Hash>::rebuild(size_t newSize) {
    tableSize = newSize;
    auto oldValues = values;
    values.clear();
    size_ = 0;
    table.assign(tableSize, {0, values.begin()});
    for(auto value : oldValues) {
        insert(value);
    }
}

template<class K, class V, class Hash>
size_t HashMap<K, V, Hash>::size() const {
    return size_;
}

template<class K, class V, class Hash> 
bool HashMap<K, V, Hash>::empty() const {
    return (size_ == 0);
}

template<class K, class V, class Hash> 
Hash HashMap<K, V, Hash>::hash_function() const {
    return hasher;
}

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::insert(const std::pair<const K, V>& value) {
    if(1.0 * size_ / tableSize > maxLoad) {
        rebuild(tableSize * 2);
    }
    auto ind = getIndex(value.first);
    auto bucketIter = table[ind].second;
    auto bucketSize = table[ind].first;
    for(size_t i = 0; i < bucketSize; ++i) {
        if(bucketIter->first == value.first) {
            return bucketIter;
        }
        ++bucketIter;
    }
    iterator it;
    if(bucketSize == 0) {
        it = values.insert(values.begin(), value);
        table[ind].second = it;
    } else {
        it = values.insert(bucketIter, value);
    }
    ++table[ind].first;
    ++size_;
    return it;
}

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::erase(const K& key) {
    if(1.0 * size_ / tableSize < minLoad && tableSize > minTableSize) {
        rebuild(tableSize / 2);
    }
    auto ind = getIndex(key);
    auto bucketIter = table[ind].second;
    auto bucketSize = table[ind].first;
    for(size_t i = 0; i < bucketSize; ++i) {
        if(bucketIter->first == key) {
            auto it = values.erase(bucketIter);
            if(i == 0) {
                table[ind].second = it;
            }
            --table[ind].first;
            --size_;
            return it;
        }
        ++bucketIter;
    }
    return end();
}

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::begin() {
    return values.begin();
}

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::end() {
    return values.end();
}

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::begin() const {
    return values.begin();
}

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::end() const {
    return values.end();
}

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::find(const K& key) const {
    auto ind = getIndex(key);
    auto bucketIter = table[ind].second;
    auto bucketSize = table[ind].first;
    for(size_t i = 0; i < bucketSize; ++i) {
        if(bucketIter->first == key) {
            return bucketIter;
        }
        ++bucketIter;
    }
    return end();
} 

template<class K, class V, class Hash> 
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::find(const K& key) {
    auto ind = getIndex(key);
    auto bucketIter = table[ind].second;
    auto bucketSize = table[ind].first;
    for(size_t i = 0; i < bucketSize; ++i) {
        if(bucketIter->first == key) {
            return bucketIter;
        }
        ++bucketIter;
    }
    return end();
} 

template<class K, class V, class Hash> 
V& HashMap<K, V, Hash>::operator [] (const K& key) {
    auto it = find(key);
    if(it == end()) {
        it = insert({key, V{}});
    }
    return it->second;
}

template<class K, class V, class Hash> 
const V& HashMap<K, V, Hash>::at(const K& key) const {
    auto it = find(key);
    if(it == end()) {
        throw std::out_of_range("Key does not exist");
    }
    return it->second;
}

template<class K, class V, class Hash>
void HashMap<K, V, Hash>::clear() {
    values.clear();
    rebuild(minTableSize);
}
