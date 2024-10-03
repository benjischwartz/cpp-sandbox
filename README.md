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

## C++ Lambda vs `std::function` vs Function Pointer

[**C++ Weekly - Ep 332 - C++ Lambda vs std::function vs Function Pointer - Jason Turner**](https://www.youtube.com/watch?v=aC-aAiS5Wuc)

- lambdas and `std::function` are **not** equivalent

**Lambda:** 

- language construct for defining an **anonymous function**
- captureless lambda is implicitly convertible to a function pointer

**Function Pointer**

- can create a **vector of function pointers**
- **captureless** lambda is **implicitly convertible** to a function pointer

```cpp
std::vector<int (*)(int, int)> operations;

// pass in a captureless lambda (converted to func pointer)
operations.emplace_back([](int x, int y){ return x + y; });  

// CAN'T do this (not captureless)
int val1 = 5;
operations.emplace_back([=](int x, int y){ return val1 + x - y; });

// pass in a function pointer
int add(int x, int y) { return x + y; }
operations.emplace_back(add);  
```

**`std::function`** 

- type-erased wrapper around a **“callable”**
- a lot of **extra overhead,** but fixes the above case
- good in the case where we need **type-erased function** which is callable on **anything** which takes that set of parameters.

```cpp
int val1 = 5;

std::vector<std::function<int, (int, int)>> operations;
operations.emplace_back([](int x, int y){ return x + y; });          // OK
operations.emplace_back([](int x, int y){ return x - y; });          // OK
operations.emplace_back([=](int x, int y){ return val1 + x - y; });  // OK
operations.emplace_back(add);                                        // OK
```

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

## Function Pointers

**syntax** is pretty straightforward: `int (*fcnPtr)();` takes no arguments and returns an int.

can **reassign function pointers** as long as the types match

```cpp
int foo() { return 5; }
int goo() { return 6; }

int main() {
	int (*fcnPtr)(){&foo};
	fcnPtr = &goo;
	
	// Dereference and call
	(*fcnPtr());
	
	// Implicit dereference
	fcnPtr();
}
```
