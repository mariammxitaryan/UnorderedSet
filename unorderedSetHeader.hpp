#ifndef UNORDERED_SET_HPP
#define UNORDERED_SET_HPP

#include <vector>
#include <functional>
#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <iterator>
#include <memory>

template<
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>,
    typename Allocator = std::allocator<Key>
>
class unordered_set {
public:
    using key_type = Key;
    using value_type = Key;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    struct Node {
        value_type value;
        Node* next;
        Node(const value_type& val) : value(val), next(nullptr) {}
        Node(value_type&& val) : value(std::move(val)), next(nullptr) {}
    };

    std::vector<Node*> buckets;
    size_type bucket_count_;
    size_type num_elements;
    float max_load_factor_;
    hasher has_func;
    key_equal key_eq;
    allocator_type alloc;

    Node* create_node(const value_type& value);
    Node* create_node(value_type&& value);
    void destroy_node(Node* node);

    void rehash_if_needed();
public:
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = unordered_set::value_type;
        using difference_type = unordered_set:difference_type;
        using pointer = value_type*;
        using reference = value_type&;

        iterator() : container(nullptr), bucket_index(0), current(nullptr) {}

        reference operator*() const {
            return current->value;
        }

        pointer operator->() const {
            return &(current->value);
        }

        iterator& operator++();
        iterator operator++(int);

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:    
        friend class unordered_set;
        unordered_set* container;
        size_type bucket_index;
        Node* current;

        iterator(unordered_set* cont, size_type index, Node* node)
            : container(cont), bucket_index(index), current(node) {}
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = unordered_set::value_type;
        using difference_type = unordered_set:difference_type;
        using pointer = const value_type*;
        using reference = const value_type&;

        iterator() : container(nullptr), bucket_index(0), current(nullptr) {}

        reference operator*() const {
            return current->value;
        }

        pointer operator->() const {
            return &(current->value);
        }

        const_iterator& operator++();
        const_iterator operator++(int);

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

    private:    
        friend class unordered_set;
        const unordered_set* container;
        size_type bucket_index;
        const Node* current;

        const_iterator(const unordered_set* cont, size_type index, const Node* node)
            : container(cont), bucket_index(index), current(node) {}
    };

    unordered_set();
    explicit unordered_set(size_type bucket_count_, const hasher& hash_func_ = hasher(), const key_equal& equal = key_equal(), const allocator_type& alloc_ = allocator_type());
    unordered_set(std::initializer_list<value_type> init, size_type bucket_count_ = 0, const hasher& hash_func_ = hasher(), const key_equal& equal = key_equal(), const allocator_type& alloc_ = allocator_type());
    unordered_set(const unordered_set& other);
    unordered_set(unordered_set&& other) noexcept;
    unordered_set& operator=(const unordered_set& other);
    unordered_set& operator=(unordered_set&& other) noexcept;
    ~unordered_set();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool empty() const;
    size_type size() const;
    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float ml);

    void clear();
    std::pair<iterator, bool> insert(const value_type& value);
    std::pair<iterator, bool> insert(value_type&& value);

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);

    iterator erase(const_iterator pos);
    size_type erase(const key_type& key);

    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;
    size_type count(const key_type& key) const;

    void rehash(size_type new_bucket_count);
    void reserve(size_type count);
};

#include "unorderedSetImplementation.tpp"

#endif