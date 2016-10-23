#include <boost/context/all.hpp>
#include <iostream>
#include <array>

namespace ctx = boost::context::detail;

class Coroutine {
public:
  Coroutine() {
    my_context = ctx::make_fcontext(
      stack.data() + stack.size(),
      stack.size(),
      &Coroutine::dispatch
    );
  }
  virtual ~Coroutine() {}

  void operator()() {
    auto transfer_ctx = ctx::jump_fcontext(my_context, this);
    my_context = transfer_ctx.fctx;
  }

protected:
  void yield() {
    auto transfer_ctx = ctx::jump_fcontext(yield_context, 0);
    my_context = transfer_ctx.fctx;
  }

  virtual void call() = 0;

private:
  static void dispatch(ctx::transfer_t coroutine_ptr) {
    Coroutine *coroutine = reinterpret_cast<Coroutine *>(coroutine_ptr.data);
    coroutine->yield_context = coroutine_ptr.fctx;
    coroutine->call();
    while(true)
      coroutine->yield();
  }

private:
  ctx::fcontext_t my_context;
  ctx::fcontext_t yield_context;
  std::array<intptr_t, 66 * 1024> stack;
};

struct A : public Coroutine {
  void call() {
    std::cout << " __________________________________ " << std::endl;
    yield();
    std::cout << "|    _       _       |_|    _| |_  |" << std::endl;
    yield();
    std::cout << "|   |_|     |_|      | |     | |_  |" << std::endl;
  }
};

struct B : public Coroutine {
  void call() {
    std::cout << "|                                  |" << std::endl;
    yield();
    std::cout << "|  _| |_   _| |_      _    |_   _| |" << std::endl;
    yield();
    std::cout << "|                    |_|     |___| |" << std::endl;
    yield();
    std::cout << "|                                  |" << std::endl;
    yield();
    std::cout << "|__________________________________|" << std::endl;
  }
};

struct C : public Coroutine {
  void call() {
    std::cout << "|                     _       _    |" << std::endl;
    yield();
    std::cout << "| |_   _| |_   _|    | |     | |   |" << std::endl;
  }

  void operator++(int) {
    std::cout << "| ++It - The Italian C++ Community |" << std::endl;
    std::cout << "|__________________________________|" << std::endl;
  }
};


int main() {

  A a;
  B b;
  C c;
  for (size_t i = 0; i<10; ++i) {
    a();
    b();
    c();
  }

  c++; // An entire operator overloading to write 'c++'? Worth it!
}