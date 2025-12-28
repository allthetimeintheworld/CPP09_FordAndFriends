# Exercise 00: Bitcoin Exchange - Reference Guide

## Overview
Create a program that calculates the value of Bitcoin on specific dates using a historical price database.

## Program Requirements

### Basic Functionality
- **Program name**: `btc`
- **Input**: Takes a file as command-line argument
- **Output**: Calculates Bitcoin value by multiplying input amount by exchange rate from database

### Usage
```bash
./btc input.txt          # Correct usage
./btc                    # Error: could not open file
```

---

## File Formats

### 1. Database File (`data.csv`)
**Purpose**: Historical Bitcoin exchange rates  
**Format**: CSV with comma separator  
**Structure**:
```csv
date,exchange_rate
2011-01-03,0.3
2011-01-09,0.32
2012-01-11,7.1
```

**Key Points**:
- Loaded internally by the program (hardcoded filename)
- NOT passed as command-line argument
- First line is header (skipped during parsing)
- Comma-separated values

### 2. Input File (`input.txt` or any name)
**Purpose**: User queries for Bitcoin values  
**Format**: Pipe-separated format  
**Structure**:
```
date | value
2011-01-03 | 3
2011-01-09 | 1
2012-01-11 | -1
2001-42-42
```

**Key Points**:
- Passed as command-line argument
- Pipe (`|`) separator between date and value
- First line is header (skipped)
- May contain invalid entries (must handle errors)

---

## STL Container Choice: `std::map`

### Why `std::map`?
```cpp
std::map<std::string, double> _data;  // Key: date, Value: exchange_rate
```

**Advantages**:
1. **Automatic Sorting**: Keys (dates) are automatically sorted in ascending order
2. **Efficient Lookup**: O(log n) search complexity
3. **Perfect for `lower_bound()`**: Essential for finding closest dates
4. **No Duplicates**: Automatically handles duplicate dates (overwrites)

### Critical Operations

#### Loading Database
```cpp
_data[date] = exchangeRate;  // Insert or update
```

#### Finding Closest Date
```cpp
std::map<std::string, double>::const_iterator it = _data.lower_bound(date);
```

**`lower_bound(date)` behavior**:
- Returns iterator to first element >= date
- If exact match exists: returns that element
- If no match: returns first element AFTER date
- To get closest LOWER date: decrement iterator once

**Example**:
```
Database: 2011-01-03, 2011-01-09, 2012-01-11
Query: 2011-01-05

lower_bound("2011-01-05") → points to "2011-01-09"
Decrement iterator        → points to "2011-01-03" ✓ (closest lower)
```

---

## Date Validation

### Valid Date Format: `YYYY-MM-DD`
```cpp
bool parseDate(const std::string& date, int& year, int& month, int& day)
```

**Checks Required**:
1. **Length**: Exactly 10 characters
2. **Separators**: Hyphens at positions 4 and 7
3. **Digits**: All other positions must be digits
4. **Month Range**: 1-12
5. **Day Range**: 1-31 (depends on month)
6. **Leap Years**: February can have 29 days

### Days in Month Logic
```cpp
int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
```

### Leap Year Calculation
```cpp
bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
```

**Examples**:
- 2000: Leap year (divisible by 400)
- 1900: Not leap year (divisible by 100 but not 400)
- 2024: Leap year (divisible by 4, not by 100)

**Invalid Dates**:
```
2001-42-42    → Invalid month (42 > 12)
2021-02-30    → Invalid day (February has max 29)
2021-13-01    → Invalid month (13 > 12)
```

---

## Value Validation

### Requirements
1. **Type**: Float or positive integer
2. **Range**: 0 to 1000 (inclusive)
3. **Must be positive**: No negative numbers

### Conversion
```cpp
char* endptr;
value = strtod(valueStr.c_str(), &endptr);

// Check if entire string was converted
if (*endptr != '\0' && *endptr != '\n')
    return false;  // Invalid format
```

### Error Cases
```cpp
if (value < 0)
    std::cerr << "Error: not a positive number." << std::endl;

if (value > 1000)
    std::cerr << "Error: too large a number." << std::endl;
```

**Examples**:
```
3         → Valid
1.2       → Valid
-1        → Error: not a positive number
2147483648 → Error: too large a number
abc       → Error: bad input
```

---

## Error Handling

### Error Types and Messages

| Scenario | Error Message |
|----------|---------------|
| No argument or file not found | `Error: could not open file.` |
| Invalid date format | `Error: bad input => [date]` |
| No pipe separator | `Error: bad input => [line]` |
| Negative value | `Error: not a positive number.` |
| Value > 1000 | `Error: too large a number.` |
| Date before database start | `Error: no data available for date => [date]` |

### Input Parsing Strategy
```cpp
size_t pipePos = line.find('|');
if (pipePos == std::string::npos) {
    // No pipe found → bad input
}

std::string date = trim(line.substr(0, pipePos));
std::string value = trim(line.substr(pipePos + 1));
```

---

## Orthodox Canonical Form

### Required Members
```cpp
class BitcoinExchange {
public:
    BitcoinExchange();                                    // Default constructor
    BitcoinExchange(const BitcoinExchange& other);       // Copy constructor
    BitcoinExchange& operator=(const BitcoinExchange& other); // Assignment operator
    ~BitcoinExchange();                                   // Destructor
};
```

### Implementation
```cpp
// Default Constructor
BitcoinExchange::BitcoinExchange() {}

// Copy Constructor - Deep copy of map
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) 
    : _data(other._data) {}

// Assignment Operator - Self-assignment check
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other)
        _data = other._data;
    return *this;
}

// Destructor
BitcoinExchange::~BitcoinExchange() {}
```

**Why std::map makes this easy**:
- std::map has its own copy constructor and assignment operator
- Automatically handles deep copying
- No manual memory management needed

---

## Key Algorithms

### 1. Trim Whitespace
```cpp
std::string trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}
```

**Why needed**: Input may have extra spaces around dates and values

### 2. String to Number Conversion
```cpp
// For integers
int value = atoi(str.c_str());

// For floating point (with validation)
char* endptr;
double value = strtod(str.c_str(), &endptr);
if (*endptr != '\0') // Not entire string converted
    return false;
```

### 3. Finding Closest Lower Date
```cpp
std::string findClosestDate(const std::string& date) const {
    std::map<std::string, double>::const_iterator it = _data.lower_bound(date);
    
    // Exact match
    if (it != _data.end() && it->first == date)
        return it->first;
    
    // No lower date exists
    if (it == _data.begin())
        return "";
    
    // Return previous element (closest lower)
    --it;
    return it->first;
}
```

**Edge Cases**:
- Query date before any database entry → return empty string
- Query date matches database entry → return that date
- Query date between entries → return the previous entry

---

## Program Flow

### 1. Main Function
```cpp
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    
    BitcoinExchange exchange;
    
    // Load database
    if (!exchange.loadDatabase("data.csv"))
        return 1;
    
    // Process input
    exchange.processInputFile(argv[1]);
    
    return 0;
}
```

### 2. Load Database
1. Open `data.csv`
2. Skip header line
3. For each line:
   - Split by comma
   - Validate date
   - Parse exchange rate
   - Store in map: `_data[date] = rate`

### 3. Process Input File
1. Open input file
2. Skip header line
3. For each line:
   - Find pipe separator
   - Extract date and value
   - Validate both
   - Find closest date in database
   - Calculate: `result = value * exchange_rate`
   - Output result or error

---

## Example Output

### Input File
```
date | value
2011-01-03 | 3
2011-01-03 | 2
2011-01-09 | 1
2012-01-11 | -1
2001-42-42
2012-01-11 | 2147483648
```

### Expected Output
```
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-09 => 1 = 0.32
Error: not a positive number.
Error: bad input => 2001-42-42
Error: too large a number.
```

---

## Common Pitfalls

### 1. ❌ Using data.csv as Input
```bash
./btc data.csv  # WRONG - will show "bad input" errors
```
**Why**: data.csv uses commas; program expects pipes

### 2. ❌ Wrong Date Comparison
```cpp
// String comparison works for YYYY-MM-DD format
"2011-01-03" < "2011-01-09"  // ✓ Correct
```
This is why std::map<std::string, double> works!

### 3. ❌ Not Handling Edge Cases
- Date before any database entry
- Empty lines
- Lines without pipe separator
- Malformed dates

### 4. ❌ Forgetting Header Line
Both files have headers that must be skipped:
```cpp
bool firstLine = true;
while (std::getline(file, line)) {
    if (firstLine) {
        firstLine = false;
        continue;  // Skip header
    }
    // Process line...
}
```

---

## Testing Strategy

### Test Cases to Cover

1. **Valid inputs**
   - Integer values
   - Float values
   - Exact date matches
   - Dates between database entries

2. **Invalid dates**
   - Wrong format (not YYYY-MM-DD)
   - Invalid month (> 12 or < 1)
   - Invalid day (> days in month)
   - Leap year edge cases

3. **Invalid values**
   - Negative numbers
   - Numbers > 1000
   - Non-numeric strings

4. **Edge cases**
   - Empty file
   - No pipe separator
   - Date before database start
   - Very large numbers

5. **File errors**
   - Non-existent file
   - No arguments

---

## Key Takeaways

1. **std::map** is perfect for sorted key-value storage with efficient lookups
2. **String dates** in YYYY-MM-DD format work perfectly with lexicographic comparison
3. **lower_bound()** is the key to finding closest dates efficiently
4. **Separate concerns**: Database (CSV) vs Input (pipe-separated)
5. **Validate everything**: Dates, values, file formats
6. **Orthodox Canonical Form** is mandatory for C++ Module 02-09
7. **Error messages** must match exactly for evaluation

---

## Verbal Breakdown: How It All Works

### The Big Picture

Imagine you have a notebook full of Bitcoin prices from different days over the years. Someone comes to you and says, "Hey, I had 3 Bitcoins on January 3rd, 2011. What was that worth?" You'd flip through your notebook, find the price for that date, multiply it by 3, and give them the answer. That's exactly what this program does, but digitally.

### The Two Files Story

**The Database (data.csv)**: Think of this as your history book. It's a CSV file (Comma-Separated Values) that looks like this:
```
date,exchange_rate
2011-01-03,0.3
2011-01-09,0.32
```

This says: "On January 3rd, 2011, one Bitcoin was worth $0.30. On January 9th, it was worth $0.32."

**The Input File (input.txt)**: This is like a list of questions people are asking you. It uses pipes (`|`) instead of commas:
```
date | value
2011-01-03 | 3
```

This is asking: "What were 3 Bitcoins worth on January 3rd, 2011?"

**Why two different formats?** Because they serve different purposes. The database is permanent historical data (comma-separated is standard for databases). The input is user queries (pipe-separated to make it easier to distinguish from the database format and avoid confusion).

### The Heart: std::map

When the program starts, it reads the entire database and stores it in a `std::map`. Why a map?

```cpp
std::map<std::string, double> _data;
// Key = "2011-01-03", Value = 0.3
```

A map is like a phone book - you look up a name (the key) and get a phone number (the value). Here, you look up a date and get the Bitcoin price.

**The magic of std::map:**
1. **It sorts automatically**: When you add "2011-01-09" and then "2011-01-03", the map automatically keeps them in order: 2011-01-03 comes first.
2. **Fast lookups**: Finding a date is super fast (logarithmic time, not linear).
3. **The lower_bound trick**: This is the superpower that makes the whole exercise work.

### The Lower Bound Trick

Here's where it gets interesting. What if someone asks: "What were my Bitcoins worth on January 5th, 2011?" But you don't have data for January 5th! The exercise says: **use the closest date that's BEFORE (lower than) the requested date**.

This is where `lower_bound()` comes in:

```cpp
std::map<std::string, double>::const_iterator it = _data.lower_bound("2011-01-05");
```

Let's say your map has:
```
2011-01-03 → 0.3
2011-01-09 → 0.32
2012-01-11 → 7.1
```

When you call `lower_bound("2011-01-05")`:
- It says: "Find the first element that is NOT LESS than (i.e., >= ) 2011-01-05"
- That would be "2011-01-09" (since it's the first date >= 2011-01-05)
- **But we want the date BEFORE that**, so we decrement the iterator once
- Now we're pointing at "2011-01-03" - perfect!

**Why string comparison works**: Because dates in "YYYY-MM-DD" format are alphabetically sortable! 
- "2011-01-03" < "2011-01-09" ✓
- "2011" < "2012" ✓
- "2011-01" < "2011-02" ✓

### The Validation Dance

Before we do any calculation, we need to make sure everything is valid. It's like being a bouncer at a club - if something doesn't meet the requirements, it doesn't get in.

**Date Validation:**
1. Must be exactly 10 characters: `YYYY-MM-DD`
2. Hyphens must be at positions 4 and 7
3. Everything else must be digits
4. Month must be 1-12
5. Day must be valid for that month (considering leap years!)

Example: `2001-42-42` fails because 42 is not a valid month.

**Value Validation:**
1. Must be a number (we use `strtod` to convert string to double)
2. Must be positive (>= 0)
3. Must not exceed 1000

Example: `-1` fails the positive test, `2147483648` fails the maximum test.

### The Processing Flow (Step by Step)

**Step 1: Program starts**
```bash
./btc input.txt
```

**Step 2: Check if we got a filename**
```cpp
if (argc != 2) {
    std::cerr << "Error: could not open file." << std::endl;
    return 1;
}
```
No filename? Error and quit.

**Step 3: Load the database**
```cpp
exchange.loadDatabase("data.csv");
```
- Open data.csv
- Skip the first line (it's just a header: "date,exchange_rate")
- For each line:
  - Split by comma
  - Left side = date, right side = price
  - Store in map: `_data["2011-01-03"] = 0.3`

**Step 4: Process the input file**
```cpp
exchange.processInputFile("input.txt");
```
- Open input.txt
- Skip the first line (header again)
- For each line:
  - Find the pipe character `|`
  - Split: left = date, right = value
  - Trim whitespace (remove extra spaces)
  - Validate date - if bad, print error and skip
  - Validate value - if bad, print error and skip
  - Find the closest date in our database
  - Calculate: `result = value × exchange_rate`
  - Print: `2011-01-03 => 3 = 0.9`

### Walking Through a Complete Example

**Database (data.csv):**
```csv
date,exchange_rate
2011-01-03,0.3
2011-01-09,0.32
2012-01-11,7.1
```

**Input (input.txt):**
```
date | value
2011-01-03 | 3
2011-01-05 | 2
2011-01-09 | 1
2012-01-11 | -1
2001-42-42
2012-01-11 | 2147483648
```

**What happens line by line:**

1. `2011-01-03 | 3`
   - Date valid? Yes
   - Value valid? Yes (3 is between 0 and 1000)
   - Find closest date: Exact match! Rate = 0.3
   - Calculate: 3 × 0.3 = 0.9
   - Output: `2011-01-03 => 3 = 0.9`

2. `2011-01-05 | 2`
   - Date valid? Yes
   - Value valid? Yes
   - Find closest date: lower_bound points to 2011-01-09, decrement → 2011-01-03. Rate = 0.3
   - Calculate: 2 × 0.3 = 0.6
   - Output: `2011-01-05 => 2 = 0.6`

3. `2011-01-09 | 1`
   - Date valid? Yes
   - Value valid? Yes
   - Find closest date: Exact match! Rate = 0.32
   - Calculate: 1 × 0.32 = 0.32
   - Output: `2011-01-09 => 1 = 0.32`

4. `2012-01-11 | -1`
   - Date valid? Yes
   - Value valid? NO! It's negative
   - Output: `Error: not a positive number.`

5. `2001-42-42`
   - No pipe separator found!
   - Output: `Error: bad input => 2001-42-42`

6. `2012-01-11 | 2147483648`
   - Date valid? Yes
   - Value valid? NO! It's > 1000
   - Output: `Error: too large a number.`

### The Orthodox Canonical Form Explained

In C++, when you create a class, the compiler automatically generates some functions for you. But for Modules 02-09, you must write them explicitly. It's called the **Orthodox Canonical Form** and includes:

1. **Default Constructor**: Creates an empty object
   ```cpp
   BitcoinExchange exchange;  // Uses default constructor
   ```

2. **Copy Constructor**: Creates a copy of an existing object
   ```cpp
   BitcoinExchange copy(exchange);  // Uses copy constructor
   ```

3. **Assignment Operator**: Assigns one object to another
   ```cpp
   BitcoinExchange another;
   another = exchange;  // Uses assignment operator
   ```

4. **Destructor**: Cleans up when object is destroyed
   ```cpp
   // Automatically called when exchange goes out of scope
   ```

**Why is this important?** It ensures you understand how objects are copied and managed in C++. For this exercise, std::map does most of the heavy lifting, so our implementations are simple. But in other exercises, you'll need to manage dynamic memory yourself.

### Common "Aha!" Moments

**"Wait, why can't I use data.csv as the input?"**
Because data.csv uses commas, but the program expects pipes in the input file. The program would try to find `|` in each line, fail, and report "bad input" for every line. The database is loaded separately - you don't pass it as an argument.

**"Why does string comparison work for dates?"**
Because YYYY-MM-DD is carefully designed to be sortable as strings:
- "2011" comes before "2012" alphabetically ✓
- "2011-01" comes before "2011-02" ✓
- "2011-01-03" comes before "2011-01-09" ✓

**"What if I query a date before any database entry?"**
The `lower_bound` would point to the very first entry. When you try to decrement to find the previous entry, you'd be at `_data.begin()`, which means there's no earlier entry. Return an empty string and show an error.

**"Why use lower_bound instead of just searching?"**
Efficiency! Searching through 1000 dates linearly would be slow. `lower_bound` uses binary search (since map is sorted) and finds the answer in O(log n) time. If you have 1000 entries, linear search = 1000 comparisons worst case, binary search = ~10 comparisons.

### The Takeaway

This exercise teaches you:
- How to work with sorted data structures (std::map)
- How to perform efficient lookups (lower_bound)
- How to validate complex inputs (dates, numbers)
- How to handle errors gracefully
- How to separate concerns (database vs queries)
- The importance of Orthodox Canonical Form

It's simulating a real-world scenario: you have historical data, users make queries, and you need to give accurate answers even when the exact data doesn't exist (by using the closest available data point).
