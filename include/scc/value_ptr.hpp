#ifndef VALUE_PTR_HPP
#define VALUE_PTR_HPP

#include "tools.hpp"

#include <utility>

template<typename T>
class ValuePtr {
private:  // Friends
    template<typename U>
    friend class ValuePtr;
private:  // Member types
    using reference = std::add_lvalue_reference_t<T>;
    static constexpr bool neDtor = std::is_nothrow_destructible_v<T>;
    static constexpr bool neCopyCtor = std::is_nothrow_copy_constructible_v<T>;
    static constexpr bool neCopyAssign = std::is_nothrow_copy_assignable_v<T>;
public:  // Member types
    using pointer = std::add_pointer_t<T>;
    using element_type = T;
    // TODO: deleter?

private:  // Member variables
    pointer m_ptr;

public:  // Constructors
    constexpr ValuePtr() SCC_THROWS requires(std::default_initializable<T>)
            : m_ptr(new T()) { }

    constexpr ValuePtr(std::nullptr_t) noexcept = delete;

    constexpr explicit ValuePtr(pointer p) noexcept
            : m_ptr(p) { }


    constexpr ValuePtr(const ValuePtr &v) SCC_THROWS requires(std::copyable<T>)
            : m_ptr(new T(*v.m_ptr)) {};

    constexpr ValuePtr(ValuePtr &&v) noexcept
            : m_ptr(v.release()) { }

    // Derived class constructor

    template<typename U>
    constexpr explicit ValuePtr(U *p) noexcept requires(std::derived_from<U, T>)
            : m_ptr(p) { }

    template<typename U>
    constexpr SCC_IMPLICIT ValuePtr(const ValuePtr<U> &v) SCC_THROWS requires(std::copyable<U> &&std::derived_from<U, T>)
            : m_ptr(new U(*v.m_ptr)) { }

    template<typename U>
    constexpr SCC_IMPLICIT ValuePtr(ValuePtr<U> &&v) noexcept requires(std::derived_from<U, T>)
            : m_ptr(v.release()) { }


    constexpr ~ValuePtr() noexcept(neDtor) {
        delete m_ptr;
    };

public:  // Assignment op
    constexpr ValuePtr &operator=(const ValuePtr &v) SCC_MUTABLE noexcept(neCopyAssign) requires(std::copyable<T>) {
        *m_ptr = *v.m_ptr;
        return *this;
    }
    constexpr ValuePtr &operator=(ValuePtr &&v) SCC_MUTABLE noexcept(neDtor) {
        reset(v.release());
        return *this;
    };
    constexpr ValuePtr &operator=(std::nullptr_t) noexcept = delete;

    // Derived
    template<typename U>
    constexpr ValuePtr &operator=(const ValuePtr<U> &v)
        SCC_MUTABLE noexcept(neCopyAssign) requires(std::copyable<T>, std::derived_from<U, T>) = delete;
    template<typename U>
    constexpr ValuePtr &operator=(ValuePtr<U> &&v) SCC_MUTABLE noexcept(neDtor) requires(std::derived_from<U, T>) {
        reset(v.release());
        return *this;
    }

public :  // Pointer API member functions
          // clang-format off
    constexpr pointer release() noexcept {
        pointer tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }
    // clang-format on
    constexpr void reset(pointer p) SCC_MUTABLE noexcept(neDtor) {
        pointer tmp = m_ptr;
        m_ptr = p;
        delete tmp;
    }
public:  // Value API member functions
    void swap(ValuePtr &v) SCC_MUTABLE noexcept {
        std::swap(m_ptr, v.m_ptr);
    }
    pointer operator->() const noexcept {
        return m_ptr;
    }

    constexpr reference get() const noexcept {
        return m_ptr;
    }
};

template<typename T, typename... Args>
ValuePtr<T> makeValue(Args &&...args) {
    return ValuePtr<T> {new T(std::forward<Args>(args)...)};
}


#endif  // VALUE_PTR_HPP
