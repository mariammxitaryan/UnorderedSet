#include "unorderedSetHeader.hpp"

/********************************************************************************
 * create_node (const value_type&)
 * ------------------------------------------------------------------------------
 * Creates a new node by copying the provided value.
 *
 * Parameters:
 *   - value: The value to be stored in the node.
 *
 * Returns:
 *   - Pointer to the newly created Node.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::Node*
unordered_set<Key, Hash, KeyEqual, Allocator>::create_node(const value_type& value) {
    return new Node(value);
}

/********************************************************************************
 * create_node (value_type&&)
 * ------------------------------------------------------------------------------
 * Creates a new node by moving the provided value.
 *
 * Parameters:
 *   - value: The rvalue reference to the value that will be moved into the node.
 *
 * Returns:
 *   - Pointer to the newly created Node.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::Node*
unordered_set<Key, Hash, KeyEqual, Allocator>::create_node(value_type&& value) {
    return new Node(std::move(value));
}

/********************************************************************************
 * destroy_node
 * ------------------------------------------------------------------------------
 * Destroys the given node, deallocating its memory.
 *
 * Parameters:
 *   - node: Pointer to the node to destroy.
 *
 * Returns:
 *   - None.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::destroy_node(Node* node) {
    delete node;
}

/********************************************************************************
 * rehash_if_needed
 * ------------------------------------------------------------------------------
 * Checks if the current load factor exceeds the maximum load factor.
 * If it does, rehashes the container by doubling the number of buckets.
 *
 * Parameters:
 *   - None.
 *
 * Returns:
 *   - None.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::rehash_if_needed() {
    if (load_factor() > max_load_factor_) {
        rehash(bucket_count_ * 2);
    }
}

/********************************************************************************
 * Default Constructor
 * ------------------------------------------------------------------------------
 * Constructs an unordered_set with default values.
 * Sets an initial bucket count of 16 and a max load factor of 1.0.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set() 
    : bucket_count_(16), num_elements(0), max_load_factor_(1.0),
      hash_func(Hash()), key_eq(KeyEqual()), alloc(Allocator())
{
    buckets.resize(bucket_count_, nullptr);
}

/********************************************************************************
 * Constructor with bucket_count_
 * ------------------------------------------------------------------------------
 * Constructs an unordered_set with a user-defined bucket count, hash functor,
 * key equality function, and allocator.
 *
 * Parameters:
 *   - bucket_count_: Initial number of buckets.
 *   - hash_func_: Hash function for keys.
 *   - equal: Equality function for keys.
 *   - alloc_: Allocator to use.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set(size_type bucket_count_, 
                                                              const hasher& hash_func_, 
                                                              const key_equal& equal, 
                                                              const allocator_type& alloc_) 
    : bucket_count_(bucket_count_ > 0 ? bucket_count_ : 16), num_elements(0), max_load_factor_(1.0),
      hash_func(hash_func_), key_eq(equal), alloc(alloc_)
{
    buckets.resize(bucket_count_, nullptr);
}

/********************************************************************************
 * Initializer List Constructor
 * ------------------------------------------------------------------------------
 * Constructs an unordered_set using an initializer_list, with an optional initial
 * bucket count, hash functor, key equality function, and allocator.
 *
 * Parameters:
 *   - init: Initializer list of values.
 *   - bucket_count_: Initial bucket count (optional).
 *   - hash_func_: Hash function.
 *   - equal: Equality function.
 *   - alloc_: Allocator.
 ********************************************************************************/
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

/********************************************************************************
 * Copy Constructor
 * ------------------------------------------------------------------------------
 * Constructs a new unordered_set as a copy of an existing one.
 *
 * Parameters:
 *   - other: The unordered_set to copy.
 ********************************************************************************/
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

/********************************************************************************
 * Move Constructor
 * ------------------------------------------------------------------------------
 * Constructs a new unordered_set by moving an existing one.
 *
 * Parameters:
 *   - other: The unordered_set to move from.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::unordered_set(unordered_set&& other) noexcept
    : buckets(std::move(other.buckets)), bucket_count_(other.bucket_count_),
      num_elements(other.num_elements), max_load_factor_(other.max_load_factor_),
      hash_func(std::move(other.hash_func)), key_eq(std::move(other.key_eq)), alloc(std::move(other.alloc))
{
    other.bucket_count_ = 0;
    other.num_elements = 0;
}

/********************************************************************************
 * Copy Assignment Operator
 * ------------------------------------------------------------------------------
 * Assigns the contents of one unordered_set to another using copying.
 *
 * Parameters:
 *   - other: The unordered_set to copy from.
 *
 * Returns:
 *   - Reference to this unordered_set.
 ********************************************************************************/
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

/********************************************************************************
 * Move Assignment Operator
 * ------------------------------------------------------------------------------
 * Assigns the contents of one unordered_set to another using move semantics.
 *
 * Parameters:
 *   - other: The unordered_set to move from.
 *
 * Returns:
 *   - Reference to this unordered_set.
 ********************************************************************************/
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

/********************************************************************************
 * Destructor
 * ------------------------------------------------------------------------------
 * Destroys the unordered_set and releases all allocated resources.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
unordered_set<Key, Hash, KeyEqual, Allocator>::~unordered_set() {
    clear();
}

/********************************************************************************
 * iterator::operator++ (Prefix)
 * ------------------------------------------------------------------------------
 * Advances the iterator to the next element in the container.
 *
 * Returns:
 *   - Reference to the updated iterator.
 ********************************************************************************/
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

/********************************************************************************
 * iterator::operator++ (Postfix)
 * ------------------------------------------------------------------------------
 * Advances the iterator to the next element, returning the original value.
 *
 * Returns:
 *   - Iterator pointing to the element before the increment.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

/********************************************************************************
 * iterator::operator== and operator!=
 * ------------------------------------------------------------------------------
 * Compares two iterators for equality and inequality.
 *
 * Returns:
 *   - true if both iterators point to the same container location, false otherwise.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::iterator::operator==(const iterator &other) const {
    return container == other.container && bucket_index == other.bucket_index && current == other.current;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
}

/********************************************************************************
 * const_iterator::operator++ (Prefix)
 * ------------------------------------------------------------------------------
 * Advances the const_iterator to the next element in the container.
 *
 * Returns:
 *   - Reference to the updated const_iterator.
 ********************************************************************************/
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

/********************************************************************************
 * const_iterator::operator++ (Postfix)
 * ------------------------------------------------------------------------------
 * Advances the const_iterator to the next element, returning its previous state.
 *
 * Returns:
 *   - Const iterator pointing to the element before the increment.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator::operator++(int) {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

/********************************************************************************
 * const_iterator::operator== and operator!=
 * ------------------------------------------------------------------------------
 * Compares two const_iterators for equality and inequality.
 *
 * Returns:
 *   - true if both const_iterators point to the same container location, false otherwise.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator::operator==(const const_iterator &other) const {
    return container == other.container && bucket_index == other.bucket_index && current == other.current;
}

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator::operator!=(const const_iterator &other) const {
    return !(*this == other);
}

/********************************************************************************
 * begin() - iterator version
 * ------------------------------------------------------------------------------
 * Returns an iterator to the first element in the unordered_set.
 *
 * Returns:
 *   - Iterator to the first element, or end() if the container is empty.
 ********************************************************************************/
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

/********************************************************************************
 * end() - iterator version
 * ------------------------------------------------------------------------------
 * Returns an iterator representing the end of the unordered_set.
 *
 * Returns:
 *   - Iterator representing the past-the-end element.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::end() {
    return iterator(this, bucket_count_, nullptr);
}

/********************************************************************************
 * begin() - const_iterator version
 * ------------------------------------------------------------------------------
 * Returns a const_iterator to the first element in the unordered_set.
 *
 * Returns:
 *   - Const iterator to the first element, or end() if the container is empty.
 ********************************************************************************/
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

/********************************************************************************
 * end() - const_iterator version
 * ------------------------------------------------------------------------------
 * Returns a const_iterator representing the end of the unordered_set.
 *
 * Returns:
 *   - Const iterator representing the past-the-end element.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::const_iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::end() const {
    return const_iterator(this, bucket_count_, nullptr);
}

/********************************************************************************
 * cbegin() and cend()
 * ------------------------------------------------------------------------------
 * Returns constant iterators to the beginning and end of the unordered_set.
 *
 * Returns:
 *   - cbegin(): Const iterator to the first element.
 *   - cend(): Const iterator to the past-the-end element.
 ********************************************************************************/
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

/********************************************************************************
 * empty
 * ------------------------------------------------------------------------------
 * Checks if the unordered_set has no elements.
 *
 * Returns:
 *   - true if empty, false otherwise.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
bool unordered_set<Key, Hash, KeyEqual, Allocator>::empty() const {
    return num_elements == 0;
}

/********************************************************************************
 * size
 * ------------------------------------------------------------------------------
 * Retrieves the number of elements in the unordered_set.
 *
 * Returns:
 *   - Number of elements stored.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
unordered_set<Key, Hash, KeyEqual, Allocator>::size() const {
    return num_elements;
}

/********************************************************************************
 * load_factor
 * ------------------------------------------------------------------------------
 * Computes the load factor of the unordered_set.
 *
 * Returns:
 *   - The ratio of num_elements to the number of buckets.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
float unordered_set<Key, Hash, KeyEqual, Allocator>::load_factor() const {
    return static_cast<float>(num_elements) / bucket_count_;
}

/********************************************************************************
 * max_load_factor (getter)
 * ------------------------------------------------------------------------------
 * Returns the current maximum load factor of the unordered_set.
 *
 * Returns:
 *   - Current max load factor.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
float unordered_set<Key, Hash, KeyEqual, Allocator>::max_load_factor() const {
    return max_load_factor_;
}

/********************************************************************************
 * max_load_factor (setter)
 * ------------------------------------------------------------------------------
 * Sets a new maximum load factor and rehashes the container if necessary.
 *
 * Parameters:
 *   - ml: The new maximum load factor.
 *
 * Returns:
 *   - None.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::max_load_factor(float ml) {
    max_load_factor_ = ml;
    rehash_if_needed();
}

/********************************************************************************
 * clear
 * ------------------------------------------------------------------------------
 * Removes all elements from the unordered_set and frees their memory.
 *
 * Returns:
 *   - None.
 ********************************************************************************/
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

/********************************************************************************
 * insert (lvalue)
 * ------------------------------------------------------------------------------
 * Inserts an element into the unordered_set by copying.
 * If the key already exists, the insertion is ignored.
 *
 * Parameters:
 *   - value: The value to insert.
 *
 * Returns:
 *   - Pair consisting of an iterator to the inserted element (or existing element)
 *     and a bool indicating whether the insertion took place.
 ********************************************************************************/
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

/********************************************************************************
 * insert (rvalue)
 * ------------------------------------------------------------------------------
 * Inserts an element into the unordered_set by moving.
 * If the key already exists, the insertion is ignored.
 *
 * Parameters:
 *   - value: The rvalue reference to the value to insert.
 *
 * Returns:
 *   - Pair consisting of an iterator to the inserted (or existing) element
 *     and a bool indicating whether the insertion took place.
 ********************************************************************************/
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

/********************************************************************************
 * emplace
 * ------------------------------------------------------------------------------
 * Inserts an element constructed in-place into the unordered_set.
 * Perfectly forwards the arguments to the element constructor.
 *
 * Parameters:
 *   - args: Arguments for constructing a new element.
 *
 * Returns:
 *   - Pair consisting of an iterator to the inserted (or existing) element
 *     and a bool indicating whether the insertion took place.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
template<typename... Args>
std::pair<typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator, bool>
unordered_set<Key, Hash, KeyEqual, Allocator>::emplace(Args&&... args) {
    value_type temp(std::forward<Args>(args)...);
    return insert(std::move(temp));
}

/********************************************************************************
 * erase (by key)
 * ------------------------------------------------------------------------------
 * Erases the element corresponding to the specified key.
 *
 * Parameters:
 *   - key: The key of the element to remove.
 *
 * Returns:
 *   - The number of elements removed (0 or 1).
 ********************************************************************************/
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

/********************************************************************************
 * erase (by const_iterator)
 * ------------------------------------------------------------------------------
 * Erases the element at the given const_iterator position.
 *
 * Parameters:
 *   - pos: Const iterator pointing to the element to erase.
 *
 * Returns:
 *   - Iterator pointing to the element following the erased element.
 ********************************************************************************/
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

/********************************************************************************
 * erase (by iterator)
 * ------------------------------------------------------------------------------
 * Erases the element at the given iterator position.
 *
 * Parameters:
 *   - pos: Iterator pointing to the element to erase.
 *
 * Returns:
 *   - Iterator pointing to the element following the erased element.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
unordered_set<Key, Hash, KeyEqual, Allocator>::erase(iterator pos) {
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

/********************************************************************************
 * find (non-const version)
 * ------------------------------------------------------------------------------
 * Searches for the element with the given key.
 *
 * Parameters:
 *   - key: The key of the element to find.
 *
 * Returns:
 *   - Iterator pointing to the found element, or end() if not found.
 ********************************************************************************/
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

/********************************************************************************
 * find (const version)
 * ------------------------------------------------------------------------------
 * Searches for the element with the given key, returning a const_iterator.
 *
 * Parameters:
 *   - key: The key of the element to find.
 *
 * Returns:
 *   - Const iterator pointing to the found element, or end() if not found.
 ********************************************************************************/
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

/********************************************************************************
 * count
 * ------------------------------------------------------------------------------
 * Counts the elements matching the given key.
 * Since unordered_set does not allow duplicate keys, this function returns 0 or 1.
 *
 * Parameters:
 *   - key: The key to count.
 *
 * Returns:
 *   - 1 if found, 0 otherwise.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
typename unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
unordered_set<Key, Hash, KeyEqual, Allocator>::count(const key_type& key) const {
    return find(key) != cend() ? 1 : 0;
}

/********************************************************************************
 * rehash
 * ------------------------------------------------------------------------------
 * Rehashes the container so that it contains at least new_bucket_count buckets.
 *
 * Parameters:
 *   - new_bucket_count: The desired number of buckets.
 *
 * Returns:
 *   - None.
 ********************************************************************************/
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

/********************************************************************************
 * reserve
 * ------------------------------------------------------------------------------
 * Reserves enough buckets to hold at least count / max_load_factor elements.
 *
 * Parameters:
 *   - count: The desired capacity in terms of the number of elements.
 *
 * Returns:
 *   - None.
 ********************************************************************************/
template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
void unordered_set<Key, Hash, KeyEqual, Allocator>::reserve(size_type count) {
    size_type new_bucket_count = static_cast<size_type>(count / max_load_factor_) + 1;
    if (new_bucket_count > bucket_count_)
        rehash(new_bucket_count);
}
