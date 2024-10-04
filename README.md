# C++ Sandbox

## Table of Contents

- [C++ Sandbox](#c++-sandbox)
  - [Action Items to improve C++](#action-items-to-improve-c++)
  - [Object Lifetime](#object-lifetime)
  - [`std::function`](#`std::function`)
  - [`unique_ptr`](#`unique_ptr`)
    - [Functions to implement](#functions-to-implement)
    - [**Basic Operations with Unique Pointers**](#**basic-operations-with-unique-pointers**)

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

## `std::function`

[Jason Turner a simplified std::function implementation](https://www.youtube.com/watch?v=xJSKk_q25oQ)

- not a lambda, not a function pointer
- basic interface is it **needs to take some function signature**

<aside>
💡

***Q: What is partial specialisation?***

- You can specialise a template for a **subset of template parameters**
- gives custom behaviour for certain types/values, gives more fine-grain control
</aside>

- We need a **type-erased wrapper** around the callable
    - We will do this with a `callable_interface` and a `callable_impl : callable_interface` which is templated

<aside>
💡

***Q: Why???***

- We need to store a `unique_ptr` to a type of `callable_interface` , which has one virtual function which is simply `call`
- As discussed in **type erasure**, we have a **concept** (defines an interface/idea) and we have a **model** (templated wrapper object which forwards the function to actual derived)
</aside>

<aside>
💡

***Q: What is going on here???***

</aside>

```cpp
function(Ret (*f)(Param...)) 
	: callable{std::make_unique<callable_impl<Ret (*)(Param...)>>(f)} {}
```

- This is the constructor for our implementation of `std_function`
- Our `std_function` has a type erased member of type: `unique_ptr<callable_interface>` , the interface has the function `call()`
    - We are initialising this type erased member
- we `make_unique` which creates a **heap allocated** `callable_impl` which performs a `std::move`

## `unique_ptr`

- Responsible for **managing another object**, and disposes of it when `unique_ptr` goes out of scope, using the **associated deleter**
    - Happens when `unique_ptr` object destroyed OR `unique_ptr` assigned to another pointer via `operator=` or `reset()`
- Can be **empty**, can manage a **single object** (allocated with`new`) or allocated array of objects (allocated with `new[]`)
- `const std::unique_ptr` means the **managed object is limited to the scope of `unique_ptr` object**. Otherwise (non- `const unique_ptr` ) ownership can be **transferred to another `unique_ptr`**
- **Move Semantics**:
    - `unique_ptr` can be moved using `std::move`, allowing ownership of the managed object to be transferred from one `unique_ptr` to another. After the move, the original `unique_ptr` is left in a valid but unspecified state (typically null).
- **Custom Deleters**:
    - You can specify a custom deleter for a `unique_ptr`, allowing for specialised destruction logic when the pointer goes out of scope.

### Functions to implement

1. Constructor/Destructor (Default, with pointer, etc.)
2. Copy Constructor and assignment (deleted)
3. Move constructor and assignment
4. Pointer access (dereference and arrow)
5. Release
6. Reset
7. Get (returns raw pointer)
8. Boolean conversion operator (check if managing an object)

### **Basic Operations with Unique Pointers**

- **Creating Unique Pointers**: Use `std::make_unique<T>()` (since C++14) for efficient creation of unique pointers.
- **Dereferencing**: You can use the `*`operator to access the underlying object or `->` to access its members.
- **Resetting**: The `reset()` function can be used to replace the managed object with a new one or set it to `nullptr`, which deletes the current object.
- **Releasing Ownership**: The `release()` function allows you to relinquish ownership of the managed object and return a raw pointer to it, leaving the `unique_ptr` null.
