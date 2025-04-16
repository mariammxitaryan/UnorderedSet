#include "unorderedSetHeader.hpp"

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::Node* unordered_set<Key, Hash, KeyEqual, Allocator>::create_node(const value_type& value) {
    return new Node(value);
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::Node* unordered_set<Key, Hash, KeyEqual, Allocator>::create_node(value_type&& value) {
    return new Node(std::move(value));
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::destroy_node(Node* node) {
    delete node;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::rehash_if_needed() {
    if (load_factor() > max_load_factor_) {
        rehash(bucket_count_ * 2);
    }
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set() 
    : bucket_count_(16), num_elements(0), max_load_factor_(1.0),
      hash_func(Hash()), key_eq(KeyEqual()), alloc(Allocator())
{
    buckets.resize(bucket_count_, nullptr);
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set(size_type bucket_count_, const hasher& hash_func_, const key_equal& equal, const allocator_type& alloc_) 
    : bucket_count_(bucket_count_ > 0 ? bucket_count_ : 16), num_elements(0), max_load_factor_(1.0),
      hash_func(hash_func_), key_eq(equal), alloc(alloc_)
{
    buckets.resize(bucket_count_, nullptr);
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set(
    std::initializer_list<value_type> init,
    size_type bucket_count_,
    const hasher& hash_func_,
    const key_equal& equal,
    const allocator_type& alloc_)
    : unordered_set(bucket_count_ ? bucket_count_ : init.size(), hash_func_, equal, alloc_)
{
    for (const auto& val : init) {
        insert(val);
    }
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set(const unordered_set& other)
    : bucket_count_(other.bucket_count_), num_elements(0),
      max_load_factor_(other.max_load_factor_), hash_func(other.hash_func),
      key_eq(other.key_eq), alloc(other.alloc)
{
    buckets.resize(bucket_count_, nullptr);
    for (const auto& val : other) {
        insert(val);
    }
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set(unordered_set&& other) noexcept
    : buckets(std::move(other.buckets)), bucket_count_(other.bucket_count_),
      num_elements(other.num_elements), max_load_factor_(other.max_load_factor_),
      hash_func(std::move(other.hash_func)), key_eq(std::move(other.key_eq)), alloc(std::move(other.alloc))
{
    other.bucket_count_ = 0;
    other.num_elements = 0;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>&
unordered_set<Key, Hash, KeyEqual, Allocator>::operator=(const unordered_set& other) {
    if (this != &other) {
        clear();
        bucket_count_ = other.bucket_count_;
        max_load_factor_ = other.max_load_factor_;
        hash_func = other.hash_func;
        key_eq = other.key_eq;
        alloc = other.alloc;
        buckets.clear();
        buckets.resize(bucket_count_, nullptr);
        for (const auto& val : other)
            insert(val);
    }
    return *this;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>&
unordered_set<Key, Hash, KeyEqual, Allocator>::operator=(unordered_set&& other) noexcept {
    if (this != &other) {
        clear();
        buckets = std::move(other.buckets);
        bucket_count_ = other.bucket_count_;
        num_elements = other.num_elements;
        max_load_factor_ = other.max_load_factor_;
        hash_func = std::move(other.hash_func);
        key_eq = std::move(other.key_eq);
        alloc = std::move(other.alloc);
        other.bucket_count_ = 0;
        other.num_elements = 0;
    }
    return *this;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::~unordered_set() {
    clear();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator&
unordered_set<Key, Hash, KeyEqual, Allocator>::iterator::operator++() {
    if (current && current->next) {
        current = current->next;
    }
    else {
        ++bucket_index;
        while (bucket_index < container->bucket_count_ && container->buckets[bucket_index] == nullptr) {
            ++bucket_index;
        }
        current = (bucket_index < container->bucket_count_) ? container->buckets[bucket_index] : nullptr;
    }
    return *this;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::iterator::operator==(const iterator &other) const {
    return container == other.container && bucket_index == other.bucket_index && current == other.current;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
}


template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator&
unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator::operator++() {
    if (current && current->next) {
        current = current->next;
    }
    else {
        ++bucket_index;
        while (bucket_index < container->bucket_count_ && container->buckets[bucket_index] == nullptr) {
            ++bucket_index;
        }
        current = (bucket_index < container->bucket_count_) ? container->buckets[bucket_index] : nullptr;
    }
    return *this;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator::operator++(int) {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator::operator==(const const_iterator &other) const {
    return container == other.container && bucket_index == other.bucket_index && current == other.current;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator::operator!=(const const_iterator &other) const {
    return !(*this == other);
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::begin() {
    for (size_type i = 0; i < bucket_count_; ++i) {
        if (buckets[i]) {
            return iterator(this, i, buckets[i]);
        }
    }
    return end();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::end() {
    return iterator(this, bucket_count_, nullptr);
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::begin() const {
    for (size_type i = 0; i < bucket_count_; ++i) {
        if (buckets[i]) {
            return const_iterator(this, i, buckets[i]);
        }
    }
    return end();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::end() const {
    return const_iterator(this, bucket_count_, nullptr);
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::cbegin() const {
    return begin();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::cend() const {
    return end();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::empty() const {
    return num_elements == 0;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
unordered_set<Key, Hash, KeyEqual, Allocator>::size() const {
    return num_elements;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
float unordered_set<Key, Hash, KeyEqual, Allocator>::load_factor() const {
    return static_cast<float>(num_elements) / bucket_count_;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
float unordered_set<Key, Hash, KeyEqual, Allocator>::max_load_factor() const {
    return max_load_factor_;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::max_load_factor(float ml) {
    max_load_factor_ = ml;
    rehash_if_needed();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::clear() {
    for (size_type i = 0; i < bucket_count_; ++i) {
        Node* current = buckets[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            destroy_node(temp);
        }
        buckets[i] = nullptr;
    }
    num_elements = 0;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
std::pair<typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator, bool>
unordered_set<Key, Hash, KeyEqual, Allocator>::insert(const value_type& value) {
    rehash_if_needed();
    size_type index = hash_func(value) % bucket_count_;
    Node* cur = buckets[index];
    while (cur) {
        if (key_eq(cur->value, value))
            return { iterator(this, index, cur), false };
        cur = cur->next;
    }
    Node* new_node = create_node(value);
    new_node->next = buckets[index];
    buckets[index] = new_node;
    ++num_elements;
    return { iterator(this, index, new_node), true };
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
std::pair<typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator, bool>
unordered_set<Key, Hash, KeyEqual, Allocator>::insert(value_type&& value) {
    rehash_if_needed();
    size_type index = hash_func(value) % bucket_count_;
    Node* cur = buckets[index];
    while (cur) {
        if (key_eq(cur->value, value))
            return { iterator(this, index, cur), false };
        cur = cur->next;
    }
    Node* new_node = create_node(std::move(value));
    new_node->next = buckets[index];
    buckets[index] = new_node;
    ++num_elements;
    return { iterator(this, index, new_node), true };
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
template<typename... Args>
std::pair<typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator, bool>
unordered_set<Key, Hash, KeyEqual, Allocator>::emplace(Args&&... args) {
    value_type temp(std::forward<Args>(args)...);
    return insert(std::move(temp));
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
unordered_set<Key, Hash, KeyEqual, Allocator>::erase(const key_type& key) {
    size_type index = hash_func(key) % bucket_count_;
    Node* current = buckets[index];
    Node* prev = nullptr;
    while (current) {
        if (key_eq(current->value, key)) {
            if (prev)
                prev->next = current->next;
            else
                buckets[index] = current->next;
            destroy_node(current);
            --num_elements;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::erase(const_iterator pos) {
    if (pos.container != this || pos.current == nullptr)
        return end();

    size_type index = pos.bucket_index;
    Node* current = buckets[index];
    Node* prev = nullptr;
    while (current) {
        if (current == pos.current) {
            if (prev)
                prev->next = current->next;
            else
                buckets[index] = current->next;
            destroy_node(current);
            --num_elements;
            iterator ret(this, index, nullptr);
            if (prev && prev->next)
                ret.current = prev->next;
            else {
                size_type next_index = index;
                do {
                    ++next_index;
                } while (next_index < bucket_count_ && buckets[next_index] == nullptr);
                ret.bucket_index = next_index;
                ret.current = (next_index < bucket_count_ ? buckets[next_index] : nullptr);
            }
            return ret;
        }
        prev = current;
        current = current->next;
    }
    return end();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::find(const key_type& key) {
    size_type index = hash_func(key) % bucket_count_;
    Node* current = buckets[index];
    while (current) {
        if (key_eq(current->value, key))
            return iterator(this, index, current);
        current = current->next;
    }
    return end();
}
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::find(const key_type& key) const {
    size_type index = hash_func(key) % bucket_count_;
    const Node* current = buckets[index];
    while (current) {
        if (key_eq(current->value, key))
            return const_iterator(this, index, current);
        current = current->next;
    }
    return end();
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
unordered_set<Key, Hash, KeyEqual, Allocator>::count(const key_type& key) const {
    return find(key) != cend() ? 1 : 0;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::rehash(size_type new_bucket_count) {
    if (new_bucket_count <= bucket_count_)
        return;
    std::vector<Node*> new_buckets;
    new_buckets.resize(new_bucket_count, nullptr);

    for (size_type i = 0; i < bucket_count_; ++i) {
        Node* current = buckets[i];
        while (current) {
            Node* next = current->next;
            size_type new_index = hash_func(current->value) % new_bucket_count;
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;
            current = next;
        }
    }
    buckets.swap(new_buckets);
    bucket_count_ = new_bucket_count;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::reserve(size_type count) {
    size_type new_bucket_count = static_cast<size_type>(count / max_load_factor_) + 1;
    if (new_bucket_count > bucket_count_)
        rehash(new_bucket_count);
}