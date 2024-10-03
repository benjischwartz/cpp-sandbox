#include <memory>

template <typename T>
class function;

template<typename Ret, typename ... Param>
class function<Ret (Param...)>
{
public:
    function(Ret (*f)(Param...))
        : callable{std::make_unique<callable_impl<Ret (*)(Param...)>>(f)} {};

    Ret operator()(Param... param) {
        return callable->call(param...);
    };
private:

    struct callable_interface {
        virtual Ret call(Param...) = 0;
        virtual ~callable_interface() = default;
    };

    template<typename Callable>
    struct callable_impl : callable_interface {
        callable_impl(Callable callable_) : callable{std::move(callable_)} {};
        Ret call(Param... param) {
            return callable(param...);
        }
        Callable callable;
    };

    std::unique_ptr<callable_interface> callable;

};

int add(int x, int y) {
    return x + y;
}

int main()
{
    function<int (int, int)> func{add};
    return func(3, 4);
}
