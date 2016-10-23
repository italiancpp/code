#include <iostream>
#include <memory>
#include <cassert>

constexpr unsigned long long operator"" _Kb(unsigned long long v) {
  return v * 1024 * sizeof(char);
}

using fcontext_t = void*;

#ifdef __linux__
constexpr const int UNIX_CONTEXT_DATA_SIZE = 0x40;
constexpr const int UNIX_CONTEXT_DATA_RIP_OFFSET = 0x30;
#else
#error "Not yet supported"
#endif

extern "C" fcontext_t jump_to_context(fcontext_t context);

void *context_stack = nullptr;
void *sp = nullptr;

fcontext_t coroutine_ctx;
fcontext_t yield_ctx;

void yield() {
  yield_ctx = jump_to_context(yield_ctx);
}


void wannabe_coroutine() {
  std::cout << "I wanna be a coroutine when I grow my stack space up\n";
  yield();
  std::cout << "Hooray!\n";
  yield();
}

void trampoline(fcontext_t ctx) {

  yield_ctx = jump_to_context(ctx);

  wannabe_coroutine();
}

fcontext_t resume_coroutine(fcontext_t ctx) {
  return jump_to_context(ctx);
}


int main() {

  // Allocate context-stack space
  context_stack = (void*)malloc(64_Kb);

  std::size_t space = UNIX_CONTEXT_DATA_SIZE + 64;
  sp = static_cast<char*>(context_stack) + 64_Kb - space;

  sp = std::align(64, UNIX_CONTEXT_DATA_SIZE, sp, space);
  assert(sp != nullptr && space >= UNIX_CONTEXT_DATA_SIZE);

  // Do make_context's work (simplified)
  // Do *NOT* try this at home (or even worse in the office)
  void** addr = reinterpret_cast<void**>(static_cast<char*>(sp) +
                                         UNIX_CONTEXT_DATA_RIP_OFFSET);
  *addr = reinterpret_cast<void*>(&trampoline);

  // In a more complex case there might be additional initialization and
  // frame adjustments going on
  coroutine_ctx = jump_to_context(sp);

  std::cout << "Continuing on main\n";

  coroutine_ctx = resume_coroutine(coroutine_ctx);

  std::cout << "Continuing on main 2\n";

  coroutine_ctx = resume_coroutine(coroutine_ctx);

  std::cout << "Finalizing\n";

  free(context_stack);

}
