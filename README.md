# C++ Sandbox

## Action Items to improve C++

1. Create a tool that helps understand **object lifetime** (use stack trace library)
2. Study the **lambda**
3. Create a **`std::function`** implementation ⇒ implement lambdas, free functions, member functions, static member functions, etc.
    1. Make **`std::function constexpr`**
4. Implement **`unique_ptr`**
5. Implement **`shared_ptr`**
6. Implement **`vector`**
7. Implement a **circular buffer** for integers using an array

## Object Lifetime

- **Move Semantics Transfer Resources, Not Object Existence**:
    - When you perform a move operation (e.g., `l2 = std::move(l1)`), the resources of `l1` (like memory or handles) are transferred to `l2`.
    - However, `l1` remains a valid object in a "moved-from" state, meaning it still exists but its internal resources may be nullified or emptied.
- **Destructors Still Run for Moved-From Objects**:
    - Even after being moved, `l1` still exists as an object. When it goes out of scope, its destructor will be called.
    - The destructor is responsible for cleaning up any remaining state in `l1`, even if `l1` is in a moved-from state.
- **Moved-From Objects Must Be in a Valid State**:
    - After a move, the moved-from object (`l1`) should be left in a valid but unspecified state, such as having null pointers or being empty.
    - This ensures that the destructor can safely clean up any resources without causing errors (like double freeing memory).
- **Explicit Use of `std::move`**:
    - `std::move` casts an object to an rvalue, signalling that it can be moved (transferred to another object).
    - Inside functions or lambdas, even when accepting an rvalue reference (`T&&`), you must still use `std::move()` to move from a local variable, as it is treated as an lvalue.
- **Move Assignment and Move Constructor**:
    - The **move constructor** is invoked when a new object is being created from an rvalue.
    - The **move assignment operator** is invoked when an existing object is being assigned from an rvalue.
