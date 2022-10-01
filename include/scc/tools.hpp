#ifndef TOOLS_HPP
#define TOOLS_HPP


#ifdef __GNUC__
[[noreturn, gnu::always_inline]] inline void unreachable() {
    __builtin_unreachable();
}
#elif defined(_MSC_VER)
[[noreturn]] __forceinline void unreachable() {
    __assume(false);
}
#else
inline void unreachable() { }
#endif


#define SCC_THROWS   noexcept(false)
#define SCC_IMPLICIT explicit(false)
#define SCC_MUTABLE

#endif  // TOOLS_HPP
