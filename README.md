# Custom Unordered Set Implementation in C++

A custom implementation of an unordered_set container in C++, designed to mimic most of the functionality provided by the C++ Standard Library’s `std::unordered_set`. This project demonstrates the creation of a hash-based container with features such as:

- **Template Parameters:** Supports customizable key type, hashing function, equality comparator, and allocator.
- **Separate Chaining Hash Table:** Uses a vector of buckets, each being a linked list of nodes.
- **Iterator & Const Iterator:** Provides forward iterators for both mutable and immutable access.
- **Full Set of Member Functions:** Constructors (default, copy, move, initializer list), assignment operators, insert/emplace, erase (by key and iterator), find, count, clear, and hash policies (rehash and reserve).

---

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Usage Example](#usage-example)
- [Build and Run](#build-and-run)

---

## Features

- **Customizable Design:** Templated implementation allowing any type for keys along with user-defined hash and equality functions.
- **Memory Management:** Utilizes helper functions to create and destroy nodes for efficient memory management.
- **Iterators:** Both `iterator` and `const_iterator` are available for traversing the container.
- **Hash Policies:** Dynamic rehashing and bucket reservation for efficient load balancing.
- **Robust Testing:** A comprehensive `main.cpp` file tests every function of the container, from insertion and deletion to iteration and lookup.

---

## Project Structure

```
├── include/
│   └── unorderedSetHeader.hpp  // Contains the declarations of unordered_set and its member functions.
├── src/
│   └── unorderedSetImplementation.tpp  // Definitions of template member functions.
├── main.cpp                   // Tester file to demonstrate and validate functionality.
└── README.md                  // This file.
```

- **unorderedSetHeader.hpp:**  
  Defines the `unordered_set` class template along with nested iterator classes and member function declarations.

- **unorderedSetImplementation.tpp:**  
  Contains the full implementation of the member functions separated from the header file for clarity and maintainability.

- **main.cpp:**  
  A test driver that exercises all aspects of your unordered_set implementation including insertion, deletion, iteration, and copying/moving operations.

---

## Getting Started

**Project Requirements:**

   - C++ compiler supporting C++11 (or later; C++14/C++17 recommended)
   - Standard build tools like `make` (optional)

**Build Instructions:**

   - **Using g++ directly:**

     ```bash
     g++ -std=c++17 main.cpp -o custom_unordered_set
     ```

   - **Using CMake (if provided):**

     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ./custom_unordered_set
     ```

---

## Usage Example

Below is a snippet from `main.cpp` demonstrating how to use the custom unordered_set:

```cpp
#include <iostream>
#include "unorderedSetHeader.hpp"

int main() {
    // Create an unordered set of integers
    unordered_set<int> us;

    // Insert elements
    us.insert(1);
    us.insert(2);
    us.insert(3);
    
    // Iterate using a range-based for loop
    for (const auto& element : us) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    // Use find and erase functions
    auto it = us.find(2);
    if (it != us.end()) {
        us.erase(it);
    }

    // Final set output
    for (const auto& element : us) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    return 0;
}
```

This example inserts values, iterates over the container, and demonstrates element removal.

---

## Build and Run

After building the project using your preferred method, simply run the generated executable. For example:

```bash
./custom_unordered_set
```

You will see output from various tests that validate the functionality of the container. Make sure your compiler flags are set appropriately to enable C++17 or later standards.
