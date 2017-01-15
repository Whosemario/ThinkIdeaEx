/*
 */

#include <iostream>
#include <memory>

template<typename Cls>
class handler_base {
public:
    virtual void call(Cls*) = 0;
};

template<typename Handler, typename Cls>
class handler_imp : public handler_base<Cls> {
public:
    virtual void call(Cls* cls) {
        (cls->*handler_)(100);
    }
public:
    Handler handler_;
};

class A;

template<typename Handler>
std::shared_ptr<handler_base<A>> store(Handler&& handler) {
    auto sp = std::make_shared<handler_imp<Handler, A>>();
    sp->handler_ = handler;
    return sp;
}

class A {
public:
    void func(int a) {
        std::cout << a << std::endl;
    }
};

int main(int args, char* argv[])
{
    std::shared_ptr<handler_base<A>> sp = store(&A::func);
    A a;
    sp->call(&a);
    return 0;
}