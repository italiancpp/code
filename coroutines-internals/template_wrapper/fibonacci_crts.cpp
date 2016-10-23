#include <iostream>
#include <scoro.hpp>

int g_fib = -1;

void fibonacci_gen() {

  int first = 1;
  g_fib = first;
  yield();

  int second = 1;
  g_fib = second;
  yield();

  for (int i = 0; i < 9; ++i) {

    int third = first + second;
    first = second;
    second = third;
    g_fib = third;
    yield();

  }

}

int main() {

  // Outputs the first 10 Fibonacci numbers

  coroutine<void(void)> coro(fibonacci_gen);

  for (int i = 0; i <= 10; ++i) {
      coro.resume();

    if(i) std::cout << g_fib << " ";
  }

}
