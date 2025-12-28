# Exercise 01: RPN Calculator - Reference Guide

## Overview
Create a program that evaluates mathematical expressions written in Reverse Polish Notation (RPN) using a stack-based algorithm.

## What is Reverse Polish Notation (RPN)?

### Normal Notation (Infix)
```
3 + 4        → Operator between operands
(1 + 2) * 3  → Needs parentheses for precedence
```

### Reverse Polish Notation (Postfix)
```
3 4 +        → Operands first, operator after
1 2 + 3 *    → No parentheses needed!
```

**Key Advantage**: No parentheses needed, no operator precedence rules, simple left-to-right evaluation.

---

## Program Requirements

### Basic Functionality
- **Program name**: `RPN`
- **Input**: Single argument containing RPN expression (space-separated tokens)
- **Output**: Result of the calculation
- **Operators**: `+ - * /`
- **Number constraint**: Input numbers < 10 (single digits), but results can be any value

### Usage
```bash
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"    # Output: 42
./RPN "7 7 * 7 -"                     # Output: 42
./RPN "1 2 * 2 / 2 * 2 4 - +"        # Output: 0
./RPN "(1 + 1)"                       # Error (brackets not allowed)
```

---

## STL Container Choice: `std::stack`

### Why `std::stack`?
```cpp
std::stack<int> _stack;
```

**RPN is DESIGNED for stack-based evaluation!**

### Stack Operations
1. **push()**: Add element to top
2. **pop()**: Remove element from top
3. **top()**: View element at top (without removing)
4. **size()**: Get number of elements
5. **empty()**: Check if stack is empty

### LIFO Principle
**Last In, First Out** - like a stack of plates:
```
Push 3:  |   |    Push 4:  | 4 |    Pop:     | 3 |
         | 3 |             | 3 |             |   |
         |___|             |___|             |___|
```

---

## RPN Evaluation Algorithm

### The Core Algorithm
```
For each token in the expression:
    If token is a number:
        Push it onto the stack
    If token is an operator:
        Pop two operands from stack (order matters!)
        Perform operation
        Push result back onto stack
    
After processing all tokens:
    Stack should contain exactly ONE value (the result)
```

### Step-by-Step Example: `"3 4 + 2 *"`

**Expression**: 3 4 + 2 *  
**Expected**: (3 + 4) * 2 = 7 * 2 = 14

| Token | Action | Stack State |
|-------|--------|-------------|
| Start | - | `[ ]` |
| `3` | Push 3 | `[3]` |
| `4` | Push 4 | `[3, 4]` |
| `+` | Pop 4, pop 3, compute 3+4=7, push 7 | `[7]` |
| `2` | Push 2 | `[7, 2]` |
| `*` | Pop 2, pop 7, compute 7*2=14, push 14 | `[14]` |
| End | Return top | **Result: 14** |

### Critical: Order of Operands

When you pop two operands for an operation:
```cpp
int b = _stack.top();  // Second operand (popped first)
_stack.pop();
int a = _stack.top();  // First operand (popped second)
_stack.pop();

result = a - b;  // NOT b - a!
```

**Example**: `"5 3 -"` should be 5 - 3 = 2, NOT 3 - 5 = -2

The stack is LIFO, so:
- First we push 5: `[5]`
- Then we push 3: `[5, 3]`
- Pop gives us 3 (was on top)
- Pop gives us 5 (was underneath)
- We need to compute 5 - 3, so a=5, b=3

---

## Complex Example Walkthrough

### Expression: `"8 9 * 9 - 9 - 9 - 4 - 1 +"`

Let's evaluate step by step:

| Token | Action | Stack | Calculation |
|-------|--------|-------|-------------|
| `8` | Push | `[8]` | - |
| `9` | Push | `[8, 9]` | - |
| `*` | Multiply | `[72]` | 8 * 9 = 72 |
| `9` | Push | `[72, 9]` | - |
| `-` | Subtract | `[63]` | 72 - 9 = 63 |
| `9` | Push | `[63, 9]` | - |
| `-` | Subtract | `[54]` | 63 - 9 = 54 |
| `9` | Push | `[54, 9]` | - |
| `-` | Subtract | `[45]` | 54 - 9 = 45 |
| `4` | Push | `[45, 4]` | - |
| `-` | Subtract | `[41]` | 45 - 4 = 41 |
| `1` | Push | `[41, 1]` | - |
| `+` | Add | `[42]` | 41 + 1 = 42 |

**Result: 42** ✓

---

## Token Validation

### Valid Number
```cpp
bool isNumber(const std::string& token) const
```

**Criteria**:
1. Not empty
2. All characters are digits (0-9)
3. Can have leading `+` or `-` sign
4. Must have at least one digit after sign

**Valid**: `"1"`, `"9"`, `"0"`  
**Invalid**: `"10"` (subject says numbers < 10, but we accept them for intermediate results), `"a"`, `""`, `"1.5"`

### Valid Operator
```cpp
bool isOperator(const std::string& token) const
```

**Valid operators**: `+`, `-`, `*`, `/`  
**Invalid**: `(`, `)`, `^`, `%`, etc.

---

## Error Handling

### Error Types

| Error Condition | Message | Example |
|----------------|---------|---------|
| Wrong number of arguments | `Error: invalid number of arguments` | `./RPN` or `./RPN "1 2" "3"` |
| Invalid token | `Error: invalid token` | `./RPN "1 2 #"` |
| Insufficient operands | `Error: insufficient operands` | `./RPN "1 +"` (need 2 operands) |
| Too many operands | `Error: invalid expression` | `./RPN "1 2 3"` (no operator) |
| Division by zero | `Error: division by zero` | `./RPN "5 0 /"` |
| Invalid token (brackets) | `Error: invalid token` | `./RPN "(1 + 1)"` |

### Exception Handling
```cpp
try {
    RPN calculator;
    int result = calculator.evaluate(argv[1]);
    std::cout << result << std::endl;
}
catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
```

All errors throw `std::runtime_error` with descriptive messages.

---

## Implementation Details

### Parsing Tokens
```cpp
std::istringstream iss(expression);
std::string token;

while (iss >> token)  // Extracts space-separated tokens
{
    // Process each token
}
```

**Key Point**: The `>>` operator automatically handles spaces as delimiters.

### Operation Execution
```cpp
void RPN::performOperation(const std::string& op)
{
    // Need at least 2 operands
    if (_stack.size() < 2)
        throw std::runtime_error("Error: insufficient operands");
    
    // Pop in correct order
    int b = _stack.top(); _stack.pop();  // Right operand
    int a = _stack.top(); _stack.pop();  // Left operand
    
    int result;
    if (op == "+")      result = a + b;
    else if (op == "-") result = a - b;
    else if (op == "*") result = a * b;
    else if (op == "/") {
        if (b == 0) throw std::runtime_error("Error: division by zero");
        result = a / b;  // Integer division
    }
    
    _stack.push(result);
}
```

### Final Validation
```cpp
if (_stack.size() != 1)
    throw std::runtime_error("Error: invalid expression");

return _stack.top();
```

After processing all tokens, the stack MUST have exactly one value:
- **Too many values**: Not enough operators
- **Zero values**: Something went wrong (shouldn't happen if logic is correct)
- **One value**: Perfect! That's our result

---

## Orthodox Canonical Form

```cpp
class RPN {
public:
    RPN();                              // Default constructor
    RPN(const RPN& other);             // Copy constructor
    RPN& operator=(const RPN& other);  // Assignment operator
    ~RPN();                            // Destructor
};
```

### Implementation
```cpp
RPN::RPN() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other)
        _stack = other._stack;
    return *this;
}

RPN::~RPN() {}
```

**Note**: std::stack handles its own memory, so we just need to copy/assign the stack member.

---

## Why Stack for RPN?

### Natural Fit
RPN was **designed** for stack-based evaluation. The algorithm is:
1. Read left to right
2. Push operands onto stack
3. When you see an operator, the operands you need are right on top of the stack!
4. Pop, compute, push result back

### No Other Structure Works as Well
- **Queue** (FIFO)? No - we need most recent values
- **Vector/Array**? Overkill - we only need top element
- **List**? Same operations, but stack makes intent clearer

### Historical Context
Many old calculators (HP calculators) used RPN because:
- Simple hardware implementation (just need a stack)
- No need to parse parentheses or precedence
- Efficient evaluation

---

## Common Pitfalls

### 1. ❌ Wrong Operand Order
```cpp
// WRONG
int a = _stack.top(); _stack.pop();
int b = _stack.top(); _stack.pop();
result = a - b;  // This is backwards!

// CORRECT
int b = _stack.top(); _stack.pop();  // Second operand
int a = _stack.top(); _stack.pop();  // First operand
result = a - b;
```

### 2. ❌ Not Checking Stack Size
```cpp
// WRONG - what if stack is empty or has only 1 element?
int b = _stack.top(); _stack.pop();
int a = _stack.top(); _stack.pop();

// CORRECT
if (_stack.size() < 2)
    throw std::runtime_error("Error: insufficient operands");
```

### 3. ❌ Forgetting Division by Zero
```cpp
// WRONG
result = a / b;

// CORRECT
if (b == 0)
    throw std::runtime_error("Error: division by zero");
result = a / b;
```

### 4. ❌ Not Validating Final Stack
```cpp
// WRONG - what if user gave "1 2 3" (three numbers, no operator)?
return _stack.top();

// CORRECT
if (_stack.size() != 1)
    throw std::runtime_error("Error: invalid expression");
return _stack.top();
```

---

## Testing Strategy

### Valid Expressions
```bash
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"   # Expected: 42
./RPN "7 7 * 7 -"                   # Expected: 42
./RPN "1 2 * 2 / 2 * 2 4 - +"      # Expected: 0
./RPN "5 1 2 + 4 * + 3 -"          # Expected: 14
./RPN "4 2 /"                       # Expected: 2
./RPN "2 3 +"                       # Expected: 5
```

### Error Cases
```bash
./RPN "1 +"                         # Error: insufficient operands
./RPN "1 2 3"                       # Error: invalid expression
./RPN "1 0 /"                       # Error: division by zero
./RPN "(1 + 1)"                     # Error: invalid token
./RPN "1 2 #"                       # Error: invalid token
./RPN                               # Error: invalid number of arguments
```

### Edge Cases
```bash
./RPN "0"                           # Expected: 0 (just one number)
./RPN "9 9 +"                       # Expected: 18 (result > 10 is OK)
./RPN "5 3 -"                       # Expected: 2 (order matters!)
./RPN "3 5 -"                       # Expected: -2 (negative result OK)
```

---

## Conversion: Infix to RPN

Understanding how to convert helps understand RPN:

| Infix | RPN | Result |
|-------|-----|--------|
| `3 + 4` | `3 4 +` | 7 |
| `3 + 4 * 2` | `3 4 2 * +` | 11 |
| `(3 + 4) * 2` | `3 4 + 2 *` | 14 |
| `8 - 3 - 2` | `8 3 - 2 -` | 3 |
| `5 * (3 + 2)` | `5 3 2 + *` | 25 |

**Key Insight**: In RPN, operations are performed in the order they appear (left to right), and the operands are whatever is on top of the stack.

---

## Key Takeaways

1. **std::stack** is the perfect container for RPN evaluation - it's what RPN was designed for
2. **LIFO** (Last In, First Out) naturally handles operand ordering
3. **Operand order matters** for subtraction and division - be careful when popping!
4. **Validation is critical** at every step: tokens, stack size, final result
5. **RPN eliminates ambiguity** - no parentheses, no precedence rules, just left-to-right
6. **Integer division** is fine for this exercise (no decimals required)
7. **Exception handling** provides clean error reporting

---

## Verbal Breakdown: The Stack in Action

### Why a Stack? The Plate Analogy

Imagine you're washing dishes. You have a clean plate, you put it on top of the stack. Another clean plate? On top. When you need a plate, you take from the top. That's LIFO - Last In, First Out.

### Walking Through an Expression

Let's do `"3 4 + 5 *"` which means `(3 + 4) * 5 = 35`

**Mental Model**: You're a calculator with a stack of papers. Each paper has a number.

1. **See "3"**: Write 3 on a paper, put it on the stack
   ```
   Stack: [3]
   ```

2. **See "4"**: Write 4 on a new paper, put it on top
   ```
   Stack: [3, 4]
   ```

3. **See "+"**: Ah, an operator! You need two numbers:
   - Take the top paper (4)
   - Take the next paper (3)
   - Calculate: 3 + 4 = 7
   - Write 7 on a new paper, put it on the stack
   ```
   Stack: [7]
   ```

4. **See "5"**: Write 5 on a paper, put it on top
   ```
   Stack: [7, 5]
   ```

5. **See "*"**: Another operator!
   - Take the top paper (5)
   - Take the next paper (7)
   - Calculate: 7 * 5 = 35
   - Write 35 on a new paper, put it on the stack
   ```
   Stack: [35]
   ```

6. **Done**: One paper left with 35 on it. That's your answer!

### Why Order Matters

Consider `"5 3 -"`:
- Push 5: `[5]`
- Push 3: `[5, 3]`
- Subtract: Take 3 (top), take 5 (next), compute **5 - 3** = 2

If you did 3 - 5, you'd get -2, which is wrong!

**Rule**: When you pop two values, the FIRST one you pop is the RIGHT operand (second in the operation), the SECOND one you pop is the LEFT operand (first in the operation).

### The Magic of RPN

**Traditional (Infix)**: `3 + 4 * 5`
- Wait, what's the answer? 23 or 35?
- You need to know that `*` has higher precedence than `+`
- Answer: 23 (because 4 * 5 happens first, then + 3)

**RPN**: Forces you to be explicit:
- `3 4 + 5 *` = (3 + 4) * 5 = 35
- `3 4 5 * +` = 3 + (4 * 5) = 23

No ambiguity! No precedence rules needed! Just follow the stack!

### The Stack Maintains Context

The beauty of the stack is that it remembers intermediate results:

`"1 2 + 3 4 + *"` means `(1 + 2) * (3 + 4)`

1. `1 2 +` → Stack: `[3]` (we remember that 1+2=3)
2. `3 4 +` → Stack: `[3, 7]` (we remember both results)
3. `*` → Stack: `[21]` (multiply the two results)

The stack is like your scratch paper - it holds partial results until you need them.

### Error Detection is Automatic

**Too few operands**: `"1 +"`
- Push 1: `[1]`
- See +: Need 2 values, only have 1 → ERROR!

**Too many operands**: `"1 2 3"`
- Push 1, 2, 3: `[1, 2, 3]`
- Done: Stack has 3 values, should have 1 → ERROR!

**Division by zero**: `"5 0 /"`
- Push 5, 0: `[5, 0]`
- Divide: 5 / 0 → ERROR!

The stack-based approach makes these errors obvious.

---

## Historical Note

RPN was developed in the 1920s but became famous with HP calculators in the 1970s. Engineers and scientists loved it because:
1. **Fewer keystrokes**: No need to press `=` or parentheses
2. **Natural for complex calculations**: You build up results step by step
3. **See intermediate results**: Each step shows you partial answers
4. **Impossible to make precedence mistakes**: The notation forces explicit ordering

Even today, many programmers prefer RPN calculators (and some Unix tools like `dc` use it) because once you understand it, it's actually more intuitive than traditional notation for complex calculations.

---

## Summary: What You Learn

This exercise teaches:
- **Stack data structure**: LIFO operations, when to use it
- **Algorithm design**: Natural fit between problem and data structure
- **Parsing**: Tokenizing strings, validating input
- **Error handling**: Comprehensive validation at every step
- **Order of operations**: Critical for non-commutative operations
- **Expression evaluation**: Alternative to tree-based evaluation
- **std::stack API**: push, pop, top, size, empty

RPN is a perfect example of how choosing the right data structure (stack) makes the algorithm trivial. With any other structure, you'd struggle; with a stack, it's elegant and simple.
