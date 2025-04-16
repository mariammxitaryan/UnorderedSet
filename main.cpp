#include "unorderedSetHeader.hpp"

int main() {
    std::cout << "Testing unordered_set implementation:" << std::endl;

    // -------------------------------
    // 1. Default Constructor & Insert
    // -------------------------------
    unordered_set<int> us;
    std::cout << "\nInserting elements using insert:" << std::endl;
    for (int i = 1; i <= 5; ++i) {
        auto result = us.insert(i);
        std::cout << "Inserted " << i << " - Success: " << result.second << std::endl;
    }
    
    // Test duplicate insertion (should return false)
    auto dup_result = us.insert(3);
    std::cout << "Attempted to insert duplicate (3) - Success: " << dup_result.second << std::endl;

    // -------------------------------
    // 2. Iterator and Const Iterator Test
    // -------------------------------
    std::cout << "\nIterating using iterator:" << std::endl;
    for (auto it = us.begin(); it != us.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Iterating using const_iterator:" << std::endl;
    const unordered_set<int>& const_us = us;
    for (auto cit = const_us.begin(); cit != const_us.end(); ++cit) {
        std::cout << *cit << " ";
    }
    std::cout << std::endl;

    // -------------------------------
    // 3. Emplace Function Test
    // -------------------------------
    std::cout << "\nTesting emplace:" << std::endl;
    auto emplace_result = us.emplace(10);
    std::cout << "Emplaced 10 - Success: " << emplace_result.second << std::endl;
    // Trying to emplace an element that already exists.
    emplace_result = us.emplace(10);
    std::cout << "Emplaced duplicate 10 - Success: " << emplace_result.second << std::endl;

    // -------------------------------
    // 4. Find & Count Functions
    // -------------------------------
    std::cout << "\nTesting find and count:" << std::endl;
    auto it_find = us.find(3);
    if (it_find != us.end())
        std::cout << "Found element 3." << std::endl;
    else
        std::cout << "Element 3 not found." << std::endl;

    std::cout << "Count for element 3: " << us.count(3) << std::endl;
    std::cout << "Count for element 42: " << us.count(42) << std::endl;

    // -------------------------------
    // 5. Erase Function Test (by key and by iterator)
    // -------------------------------
    std::cout << "\nTesting erase by key:" << std::endl;
    size_t erased_count = us.erase(2);
    std::cout << "Erased element 2, count: " << erased_count << std::endl;
    
    std::cout << "Erasing element 42 (non-existent): " << us.erase(42) << std::endl;

    std::cout << "\nTesting erase by iterator:" << std::endl;
    auto itToErase = us.find(3);
    if (itToErase != us.end()) {
        us.erase(itToErase);
        std::cout << "Erased element 3 using iterator." << std::endl;
    }
    
    // Print contents after erase operations.
    std::cout << "Contents after erases:" << std::endl;
    for (const auto &e : us)
        std::cout << e << " ";
    std::cout << std::endl;

    // -------------------------------
    // 6. Capacity and Hash Policy
    // -------------------------------
    std::cout << "\nTesting capacity and hash policy functions:" << std::endl;
    std::cout << "Size: " << us.size() << std::endl;
    std::cout << "Empty: " << (us.empty() ? "Yes" : "No") << std::endl;
    std::cout << "Load factor: " << us.load_factor() << std::endl;
    std::cout << "Max load factor: " << us.max_load_factor() << std::endl;

    // Change max load factor and test reserve/rehash
    us.max_load_factor(0.5);
    std::cout << "Updated max load factor to: " << us.max_load_factor() << std::endl;
    us.reserve(20);
    std::cout << "Reserved space for 20 elements. New load factor: " << us.load_factor() << std::endl;

    // -------------------------------
    // 7. Clear Function Test
    // -------------------------------
    std::cout << "\nTesting clear:" << std::endl;
    us.clear();
    std::cout << "After clear, size: " << us.size() << ", empty: " << (us.empty() ? "Yes" : "No") << std::endl;

    // -------------------------------
    // 8. Copy and Move Constructors / Assignment
    // -------------------------------
    std::cout << "\nTesting copy constructor and assignment operators:" << std::endl;
    // Populate a new unordered_set.
    for (int i = 100; i < 105; ++i) {
        us.insert(i);
    }
    std::cout << "Original set:" << std::endl;
    for (const auto& item : us)
        std::cout << item << " ";
    std::cout << std::endl;

    // Copy constructor.
    unordered_set<int> us_copy(us);
    std::cout << "Copied set:" << std::endl;
    for (const auto& item : us_copy)
        std::cout << item << " ";
    std::cout << std::endl;

    // Move constructor.
    unordered_set<int> us_move(std::move(us_copy));
    std::cout << "Moved set:" << std::endl;
    for (const auto& item : us_move)
        std::cout << item << " ";
    std::cout << std::endl;

    // Copy assignment operator.
    unordered_set<int> us_assign;
    us_assign = us_move;
    std::cout << "After copy assignment, assigned set:" << std::endl;
    for (const auto& item : us_assign)
        std::cout << item << " ";
    std::cout << std::endl;

    // Move assignment operator.
    unordered_set<int> us_moveAssign;
    us_moveAssign = std::move(us_assign);
    std::cout << "After move assignment, moved-assigned set:" << std::endl;
    for (const auto& item : us_moveAssign)
        std::cout << item << " ";
    std::cout << std::endl;

    std::cout << "\nAll tests completed successfully." << std::endl;
    return 0;
}
