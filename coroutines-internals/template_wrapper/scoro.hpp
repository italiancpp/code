/* ******************************************************************
   SCORO: Simple COROutine
   Author: Marco Alesiani

   This code was written for didactic purposes - no sane programmer
   would think of using it in a production environment.
   Use boost::coroutines2 instead.
   You've been warned.

   License: CC-4.0 https://creativecommons.org/licenses/by/4.0/

****************************************************************** */

#ifndef SCORO_HEADER_HPP
#define SCORO_HEADER_HPP

#include <memory>
#include <cassert>
#include <vector>
#include <functional>

inline constexpr unsigned long long operator"" _Kb(unsigned long long v) {
  return v * 1024 * sizeof(char);
}

using fcontext_t = void*;

#ifdef __linux__
constexpr const int UNIX_CONTEXT_DATA_SIZE = 0x40;
constexpr const int UNIX_CONTEXT_DATA_RIP_OFFSET = 0x30;
#else
#error "Not yet supported"
#endif

extern "C" fcontext_t jump_to_context(void *this_ptr, fcontext_t context);

// A stack of fcontexts. Simple abstraction that won't work in multiple TUs.
static std::vector<fcontext_t> yield_ctx;

inline void yield() {
  size_t index = yield_ctx.size() - 1;
  yield_ctx[index] = jump_to_context(nullptr, yield_ctx[index]);
}

template<typename Fn> class coroutine;

// Needed to deal with pmf-conversions
template <class Fn>
void call_member_trampoline(coroutine<Fn> *instance, fcontext_t ctx);

template<typename Fn>
class coroutine {
  void *context_stack = nullptr;
  Fn *m_fn;
  fcontext_t coroutine_ctx;

  template <class TFn>
  friend void call_member_trampoline(coroutine<TFn> *instance, fcontext_t ctx);

  void trampoline(fcontext_t ctx) {

    size_t index = yield_ctx.size() - 1;
    yield_ctx[index] = jump_to_context(this, ctx);

    this->m_fn();
  }

public:
  coroutine(Fn&& fn) : m_fn(fn) {

    // Allocate context-stack space
    context_stack = (void*)malloc(64_Kb);

    void *sp = nullptr;
    std::size_t space = UNIX_CONTEXT_DATA_SIZE + 64;
    sp = static_cast<char*>(context_stack) + 64_Kb - space;

    sp = std::align(64, UNIX_CONTEXT_DATA_SIZE, sp, space);
    assert(sp != nullptr && space >= UNIX_CONTEXT_DATA_SIZE);


    // Do make_context's work (simplified)
    // Do *NOT* try this at home (or even worse in the office)
    void** addr = reinterpret_cast<void**>(static_cast<char*>(sp) +
                                           UNIX_CONTEXT_DATA_RIP_OFFSET);
    *addr = reinterpret_cast<void*>(&call_member_trampoline<Fn>);

    yield_ctx.emplace_back();

    // In a more complex case there might be additional initialization and
    // frame adjustments going on
    coroutine_ctx = jump_to_context(this, sp);
  }

  ~coroutine() {
    free(context_stack);
    yield_ctx.pop_back();
  }

  void resume() {
    coroutine_ctx = jump_to_context(nullptr, coroutine_ctx);
  }
};

template <class Fn>
void call_member_trampoline(coroutine<Fn> *instance, fcontext_t ctx) {
  instance->trampoline(ctx);
}

#endif // SCORO_HEADER_HPP
