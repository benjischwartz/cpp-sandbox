#include <iostream>
#include <utility>
#include <cassert>

template<typename T>
class UniquePtr 
{
public:
    // Default Constructor
    explicit UniquePtr(T* ptr_ = nullptr) : ptr(ptr_) {}

    // Copy Constructor and Assignment (deleted)
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Destructor
    ~UniquePtr() noexcept { delete ptr; }

    // Move Constructor
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // Move Assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T* get() const noexcept { return ptr; }

    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* ptr_ = nullptr) noexcept {
        delete ptr;
        ptr = ptr_;
    }

    T& operator*() const noexcept { return *ptr; }

    T* operator->() const noexcept { return ptr; }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
    
private:
    T* ptr;
};

class TestClass {
public:
    TestClass(int value) : value(value) {}
    int value;
};

int main() {

    // Default Constructor
    UniquePtr<TestClass> ptr1;
    assert(!ptr1);

    // Construct with raw pointer
    UniquePtr<TestClass> ptr2(new TestClass(42));
    assert(ptr2);
    assert(ptr2->value == 42);

    // Move constructor
    UniquePtr<TestClass> ptr3 = std::move(ptr2);
    assert(!ptr2);
    assert(ptr3);
    assert(ptr3->value == 42);

    // Move assignment
    UniquePtr<TestClass> ptr4;
    ptr4 = std::move(ptr3);
    assert(!ptr3);
    assert(ptr4);
    assert(ptr4->value == 42);

    // Release ownership
    TestClass* rawPtr = ptr4.release();
    assert(!ptr4);
    assert(rawPtr->value == 42);
    delete rawPtr;

    // Reset with new pointer
    UniquePtr<TestClass> ptr5(new TestClass(10));
    assert(ptr5->value == 10);
    ptr5.reset(new TestClass(20));
    assert(ptr5->value == 20);
    ptr5.reset();
    assert(!ptr5);

    // Reset to new pointer and delete
    ptr5.reset(new TestClass(30));
    assert(ptr5->value == 30);

    // Test bool operator
    assert(static_cast<bool>(ptr5));
    ptr5.reset();
    assert(!static_cast<bool>(ptr5));

    std::cout << "All tests passed!\n";
    return 0;
}
