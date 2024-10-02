#include <cstdio>
#include <memory>

struct Lifetime {
    // Default constructor
    Lifetime() noexcept { puts("Lifetime() [Default constructor]"); }

    // Copy constructor
    Lifetime(const Lifetime&) noexcept { puts("Lifetime(const Lifetime&) [Copy constructor]"); }

    // Move constructor
    Lifetime(Lifetime&&) noexcept { puts("Lifetime(Lifetime&&) [Move constructor]"); }

    // Destructor
    ~Lifetime() noexcept { puts("~Lifetime() [Destructor]"); }

    // Copy assignment
    Lifetime& operator=(const Lifetime&) noexcept { 
        puts("operator=(const Lifetime&) [Copy assignment]"); 
        return *this;
    }

    // Move assignment
    Lifetime& operator=(Lifetime&&) noexcept { 
        puts("operator=(Lifetime&&) [Move assignment]"); 
        return *this;
    }
        
};

Lifetime f1() {
    return Lifetime{};
}

Lifetime f2() {
    return f1();
}

int main()
{
    //Lifetime l = f2();
    {
        puts("Test 1");
        auto l2 = std::make_unique<Lifetime>();
        Lifetime l3 = *l2;
        auto lm = [](Lifetime&& l){ Lifetime l4 = l; puts("This is a lambda"); };
        lm(std::move(l3));
    }

    {
        puts("\nTest 2");
        Lifetime l;
        Lifetime l2 = std::move(l);
        auto lm = [&l2](){ Lifetime l3 = std::move(l2); puts("This is first lambda"); };
        auto lm2 = [&l2](){ Lifetime l3 = l2; puts("This is second lambda"); };
        lm();
        lm2();
    }

    {
        puts("\nTest 3");
        Lifetime l;
        auto lm3 = [](Lifetime& l){ Lifetime l3; l3 = std::move(l); puts("This is a lambda"); };
        lm3(l);
    }

    {
        puts("\nTest 4");
        Lifetime l1, l2;
        l2 = std::move(l1);
    }

    {
        puts("\nTest 5");
        Lifetime l1;
        auto lm = [&l1](){ Lifetime l2; l2 = std::move(l1); puts("This is a lambda"); };
        lm();
    }

    {
        puts("\nTest 6");
        class myStruct {
        public:
            Lifetime& func() { return l; };

        private:
            Lifetime l;
        };

        myStruct ms {};
        Lifetime l2;
        l2 = std::move(ms.func());
    }
}
