# Exercise 02: PmergeMe - Reference Guide

## Overview
Implement the Ford-Johnson merge-insert sort algorithm (one of the most comparison-efficient sorting algorithms) using two different STL containers and compare their performance.

## What is Ford-Johnson (Merge-Insert) Sort?

The **Ford-Johnson algorithm** (also called **merge-insertion sort**) was published in 1959 and holds the record for minimizing the number of comparisons needed to sort a sequence. It's mentioned in Donald Knuth's "The Art of Computer Programming, Vol. 3, Page 184."

### Why is it Special?
- **Minimizes comparisons**: Uses fewer comparisons than merge sort or quicksort
- **Optimal for small lists**: Achieves near-optimal comparison count
- **Complex to implement**: More complicated than standard sorting algorithms
- **Historical significance**: Was the best known sorting algorithm for decades

---

## Program Requirements

### Basic Functionality
- **Program name**: `PmergeMe`
- **Input**: Positive integer sequence as command-line arguments
- **Algorithm**: Ford-Johnson merge-insert sort
- **Containers**: Must use TWO different containers (vector and deque)
- **Capacity**: Handle at least 3000 integers
- **Output**: Display before/after sequences and timing for both containers

### Usage
```bash
./PmergeMe 3 5 9 7 4
# Before: 3 5 9 7 4
# After: 3 4 5 7 9
# Time to process a range of 5 elements with std::vector : 0.00031 us
# Time to process a range of 5 elements with std::deque : 0.00014 us

./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
# Before: 141 79 526 321 [...]
# After: 79 141 321 526 [...]
# Time to process a range of 3000 elements with std::vector : 62.14389 us
# Time to process a range of 3000 elements with std::deque : 69.27212 us

./PmergeMe "-1" "2"
# Error
```

---

## STL Container Choices

### Why std::vector and std::deque?

We can't use:
- ❌ `std::map` (used in ex00)
- ❌ `std::stack` (used in ex01)

Best choices for this algorithm:
- ✅ `std::vector` - Dynamic array, excellent cache locality
- ✅ `std::deque` - Double-ended queue, good for insertions at both ends

### std::vector
```cpp
std::vector<int> _vectorData;
```

**Characteristics**:
- Contiguous memory (like array)
- Fast random access: O(1)
- Insert at end: O(1) amortized
- Insert in middle: O(n) (must shift elements)
- Excellent cache performance
- **Best for**: Sequential access, random access, back insertions

### std::deque
```cpp
std::deque<int> _dequeData;
```

**Characteristics**:
- Non-contiguous memory (chunks of arrays)
- Fast random access: O(1)
- Insert at both ends: O(1)
- Insert in middle: O(n)
- Good for front/back operations
- **Best for**: Operations at both ends, when front insertion needed

### Performance Comparison
```
Operation       | vector | deque
----------------|--------|-------
Random Access   | O(1)   | O(1)
Push Back       | O(1)*  | O(1)
Push Front      | O(n)   | O(1)  ← deque wins
Insert Middle   | O(n)   | O(n)
Memory Layout   | Contiguous | Chunked
Cache Locality  | Excellent | Good
```

---

## Ford-Johnson Algorithm Explained

### High-Level Strategy

The algorithm works in phases:

1. **Pair up elements** and compare within each pair
2. **Sort pairs** by their larger element
3. **Build main chain** from larger elements (guaranteed sorted)
4. **Insert smaller elements** using binary search with optimal ordering

### Why It's Efficient

Traditional merge sort: n log n comparisons  
Ford-Johnson: **Close to theoretical minimum** of log₂(n!) comparisons

### Step-by-Step Algorithm

#### Phase 1: Create and Sort Pairs

```
Input: [5, 3, 8, 1, 9, 2, 7, 6]

Step 1: Pair up elements
Pairs: (5,3) (8,1) (9,2) (7,6)

Step 2: Order each pair (smaller, larger)
Pairs: (3,5) (1,8) (2,9) (6,7)

Step 3: Sort pairs by larger element
Sorted: (3,5) (6,7) (1,8) (2,9)
        └─┘   └─┘   └─┘   └─┘
         5     7     8     9
```

#### Phase 2: Build Main Chain

```
Extract larger elements (already sorted):
Main Chain: [5, 7, 8, 9]

Extract smaller elements (to be inserted):
Pend: [3, 6, 1, 2]
```

#### Phase 3: Insert First Pend Element

```
Insert pend[0] at the beginning:
Main Chain: [3, 5, 7, 8, 9]

Why? Because we know 3 < 5 (from original pair),
and 5 is the smallest in main chain.
```

#### Phase 4: Binary Insert Remaining Elements

```
Insert pend[1] = 6:
  Binary search in [3, 5, 7, 8, 9]
  Result: [3, 5, 6, 7, 8, 9]

Insert pend[2] = 1:
  Binary search in [3, 5, 6, 7, 8, 9]
  Result: [1, 3, 5, 6, 7, 8, 9]

Insert pend[3] = 2:
  Binary search in [1, 3, 5, 6, 7, 8, 9]
  Result: [1, 2, 3, 5, 6, 7, 8, 9]

Final: [1, 2, 3, 5, 6, 7, 8, 9] ✓
```

### Handling Odd Number of Elements

```
Input: [5, 3, 8, 1, 9]  ← 5 elements (odd)

Pairs: (3,5) (1,8)
Straggler: 9  ← leftover element

After building main chain and inserting pend elements,
insert straggler using binary search.
```

---

## Implementation Details

### Algorithm Structure

```cpp
void mergeInsertSortVector(std::vector<int>& arr)
{
    // 1. Base case: small arrays use insertion sort
    if (n <= 10) {
        insertionSort(arr);
        return;
    }
    
    // 2. Create pairs and order them
    for (i = 0; i + 1 < n; i += 2) {
        pairs.push_back(make_pair(min, max));
    }
    
    // 3. Handle odd element (straggler)
    if (n % 2 == 1)
        straggler = arr[n-1];
    
    // 4. Sort pairs by larger element
    sort(pairs by second element);
    
    // 5. Build main chain (larger elements)
    // 6. Build pend (smaller elements)
    
    // 7. Insert first pend at beginning
    mainChain.insert(begin, pend[0]);
    
    // 8. Binary insert remaining pend elements
    for (each pend[i]) {
        pos = lower_bound(mainChain, pend[i]);
        mainChain.insert(pos, pend[i]);
    }
    
    // 9. Binary insert straggler if exists
    if (straggler exists) {
        pos = lower_bound(mainChain, straggler);
        mainChain.insert(pos, straggler);
    }
}
```

### Using Binary Search (std::lower_bound)

```cpp
std::vector<int>::iterator pos = std::lower_bound(
    mainChain.begin(),
    mainChain.end(),
    elementToInsert
);
mainChain.insert(pos, elementToInsert);
```

**What lower_bound does**:
- Returns iterator to first element NOT LESS than value
- Uses binary search: O(log n)
- Perfect for inserting into sorted sequence

### Insertion Sort for Small Arrays

```cpp
void insertionSort(std::vector<int>& arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
```

**Why use insertion sort for small arrays?**
- Fewer comparisons than recursive approach
- Better cache performance
- Lower overhead
- Threshold of 10 is empirically good

---

## Timing Measurement

### Using gettimeofday

```cpp
#include <sys/time.h>

struct timeval start, end;

gettimeofday(&start, NULL);
// ... perform sorting ...
gettimeofday(&end, NULL);

double time = (end.tv_sec - start.tv_sec) * 1000000.0;
time += (end.tv_usec - start.tv_usec);

std::cout << "Time: " << time << " us" << std::endl;
```

**Why microseconds (us)?**
- More precise than milliseconds for small datasets
- Can measure even very fast sorts
- Standard unit for algorithm timing

### What to Measure

Include EVERYTHING:
- Parsing/copying data into container
- Actual sorting algorithm
- Any auxiliary operations

**Don't measure**:
- Input parsing from command line (only once)
- Output display

---

## Input Validation

### Valid Input
```cpp
bool isValidNumber(const std::string& str)
{
    // Must be non-empty
    if (str.empty()) return false;
    
    // All characters must be digits
    for (char c : str)
        if (!isdigit(c)) return false;
    
    // Must be positive (> 0)
    long num = atol(str.c_str());
    if (num <= 0) return false;
    
    // Must fit in int range
    if (num > 2147483647) return false;
    
    return true;
}
```

### Error Cases
```bash
./PmergeMe                  # Error: no input
./PmergeMe "-1"            # Error: negative
./PmergeMe "1" "2" "abc"   # Error: not a number
./PmergeMe "0"             # Error: not positive
```

### Duplicates
The subject says: "The management of errors related to duplicates is left to your discretion."

**Options**:
1. Allow duplicates (simplest)
2. Reject duplicates
3. Remove duplicates

Most implementations allow duplicates.

---

## Complete Example Walkthrough

### Input: `"3 5 9 7 4"`

**Step 1: Parse Input**
```
Vector: [3, 5, 9, 7, 4]
Deque:  [3, 5, 9, 7, 4]
```

**Step 2: Create Pairs**
```
(3, 5) → order: (3, 5)
(9, 7) → order: (7, 9)
Straggler: 4
```

**Step 3: Sort Pairs by Larger Element**
```
Pairs: (3, 5), (7, 9)  ← already sorted (5 < 9)
```

**Step 4: Build Chains**
```
Main Chain: [5, 9]    ← larger elements
Pend:       [3, 7]    ← smaller elements
```

**Step 5: Insert pend[0] at Beginning**
```
Main Chain: [3, 5, 9]
```

**Step 6: Binary Insert pend[1] = 7**
```
lower_bound([3, 5, 9], 7) → position before 9
Insert 7: [3, 5, 7, 9]
```

**Step 7: Binary Insert Straggler = 4**
```
lower_bound([3, 5, 7, 9], 4) → position before 5
Insert 4: [3, 4, 5, 7, 9]
```

**Result: [3, 4, 5, 7, 9]** ✓

---

## Performance Analysis

### Time Complexity
- **Best/Average/Worst**: O(n log n)
- **Comparisons**: Close to theoretical minimum log₂(n!)
- **Better than**: Quicksort, merge sort (in terms of comparisons)

### Space Complexity
- **O(n)** for auxiliary storage (pairs, chains)

### Practical Performance

For small arrays (< 50 elements):
- Ford-Johnson: Fewer comparisons than quicksort
- May be slower in wall-clock time (more overhead)

For large arrays (> 1000 elements):
- Quicksort usually faster (better cache behavior)
- Ford-Johnson still uses fewer comparisons

### Vector vs Deque Performance

**Expected**:
- **Vector usually faster**: Better cache locality
- **Deque competitive**: Especially with many insertions

**Factors**:
- Memory layout (contiguous vs chunked)
- Cache line efficiency
- Insertion patterns
- Compiler optimizations

---

## Common Pitfalls

### 1. ❌ Not Handling Odd Number of Elements
```cpp
// WRONG - what if n is odd?
for (int i = 0; i < n; i += 2) {
    pairs.push_back(...);
}

// CORRECT
for (i = 0; i + 1 < n; i += 2) {
    pairs.push_back(...);
}
if (i < n)  // Handle leftover
    straggler = arr[i];
```

### 2. ❌ Not Sorting Pairs Correctly
```cpp
// WRONG - need to sort by LARGER element
std::sort(pairs.begin(), pairs.end());

// CORRECT - custom comparison
// Or manually sort by second element
```

### 3. ❌ Forgetting to Insert First Pend at Beginning
```cpp
// The first pend element is special!
// It's guaranteed smaller than mainChain[0]
mainChain.insert(mainChain.begin(), pend[0]);
```

### 4. ❌ Not Including All Operations in Timing
```cpp
// WRONG
copy data to container;
start_timer();
sort();
stop_timer();

// CORRECT
start_timer();
copy data to container;
sort();
stop_timer();
```

### 5. ❌ Using Wrong Iterator Types
```cpp
// WRONG - std::vector vs std::deque have different iterator types
std::vector<int>::iterator it = deque.begin();  // Error!

// CORRECT - use appropriate iterator type
std::deque<int>::iterator it = deque.begin();
```

---

## Testing Strategy

### Small Datasets
```bash
./PmergeMe 3 5 9 7 4
./PmergeMe 1
./PmergeMe 1 2
./PmergeMe 5 4 3 2 1          # Reverse sorted
./PmergeMe 1 2 3 4 5          # Already sorted
./PmergeMe 2 2 2 2 2          # All same (if allowing duplicates)
```

### Medium Datasets
```bash
./PmergeMe 3 5 9 7 4 2 8 6 1 10
./PmergeMe `seq 1 100 | shuf`
```

### Large Datasets (3000+ elements)
```bash
# Linux
./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
./PmergeMe `shuf -i 1-100000 -n 5000 | tr "\n" " "`

# macOS
./PmergeMe `jot -r 3000 1 100000 | tr '\n' ' '`
```

### Error Cases
```bash
./PmergeMe                    # No arguments
./PmergeMe "-1"              # Negative
./PmergeMe "1" "2" "abc"     # Invalid character
./PmergeMe "0"               # Zero (not positive)
./PmergeMe "99999999999999"  # Too large
```

### Validation
```bash
# Sort and verify output is correct
./PmergeMe 9 3 5 1 7 | grep "After:" | \
    grep -q "1 3 5 7 9" && echo "PASS" || echo "FAIL"
```

---

## Orthodox Canonical Form

```cpp
class PmergeMe {
public:
    PmergeMe();                              // Default constructor
    PmergeMe(const PmergeMe& other);        // Copy constructor
    PmergeMe& operator=(const PmergeMe& other);  // Assignment
    ~PmergeMe();                            // Destructor
    
private:
    std::vector<int> _vectorData;
    std::deque<int> _dequeData;
};
```

### Implementation
```cpp
PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) 
    : _vectorData(other._vectorData), 
      _dequeData(other._dequeData) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vectorData = other._vectorData;
        _dequeData = other._dequeData;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}
```

---

## Key Takeaways

1. **Ford-Johnson** minimizes comparisons but has complexity in implementation
2. **std::vector** and **std::deque** both support random access needed for this algorithm
3. **Binary search** (lower_bound) is crucial for efficient insertion
4. **Pairs and main chain** are the core data structures of the algorithm
5. **Timing precision** requires microseconds for meaningful comparison
6. **Handle edge cases**: odd numbers, single element, already sorted
7. **Two implementations** required - good exercise in code reuse and container differences
8. **Performance comparison** shows real-world container behavior

---

## Verbal Breakdown

### The Core Idea

Imagine you're organizing a tournament but you want to minimize the number of matches. Ford-Johnson is like that - it's a clever way to sort that reduces the number of comparisons.

### The Pairs Trick

Instead of comparing everything to everything, you:
1. Pair up elements and find winners
2. Sort the winners (they're already partially sorted!)
3. Smartly insert the losers using what you already know

It's like having a bracket tournament, then using that information to place everyone else.

### Why It's Hard

Most sorting algorithms are simple to understand:
- Bubble sort: Keep swapping neighbors
- Quick sort: Pick a pivot, partition
- Merge sort: Split, sort, merge

Ford-Johnson is clever but complex:
- Multiple phases with different logic
- Special handling for edge cases
- Requires understanding of optimal insertion ordering

### The Payoff

For small lists, Ford-Johnson uses the MINIMUM possible comparisons (or very close). That's powerful for situations where comparisons are expensive (like comparing large strings or database records).

### Vector vs Deque in Practice

**Vector**: "I'm a contiguous array in memory. Super fast when you're going through me sequentially because I fit nicely in CPU cache."

**Deque**: "I'm made of chunks. I'm almost as fast as vector for most things, and I'm better when you need to insert at the front!"

For this algorithm, vector usually wins because we mostly do sequential access and insertions in the middle (where both are O(n) anyway).

This exercise teaches you that **choosing the right container matters**, and **theoretical optimality** (fewer comparisons) doesn't always mean faster in practice (cache effects, overhead).
