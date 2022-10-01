#include "tools.hpp"

#include <algorithm>
#include <cstring>
#include <dktt/dktt.hpp>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

template<typename...>
struct Head;

template<typename T, typename... Rest>
struct Head<T, Rest...> {
    using type = T;
};

template<typename... Rest>
using Head_t = typename Head<Rest...>::type;

template<typename V>
struct UniquePtrVectorIterator {
private:
    using This = UniquePtrVectorIterator<V>;
public:
    using difference_type = typename V::difference_type;
    using iterator_category = std::random_access_iterator_tag;
    using iterator_concept = std::contiguous_iterator_tag;
    using pointer = typename V::pointer;
    using reference = typename V::reference;
    using value_type = typename V::value_type;
private:
    pointer ptr = nullptr;
public:
    reference operator*() {
        return *ptr;
    }

    value_type operator*() const {
        return *ptr;
    }

    This &operator++() {
        ++ptr;
        return *this;
    }

    This operator++(int) {
        const auto tmp = this;
        ptr++;
        return this;
    }
};

template<typename V>
struct ConstUniquePtrVectorIterator : UniquePtrVectorIterator<V> {
    using pointer = typename V::const_pointer;
    using reference = typename V::const_reference;
    using value_type = typename V::const_value_type;
};



template<typename T>
class UniquePtrVector {
private:  // Member types
    using This = UniquePtrVector<T>;

    template<typename U>
    using is_unique_ptr = dktt::is_template_instance<std::unique_ptr, U>;
public:  // Member types
    using value_type = T *;
    using const_value_type = const T *;            // not standard
    using unique_value_type = std::unique_ptr<T>;  // not standard
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = UniquePtrVectorIterator<This>;
    using const_iterator = ConstUniquePtrVectorIterator<This>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


private:  // Member vars
    pointer m_data;
    size_type m_size;
    size_type m_cap;


private:  // Constructors
    UniquePtrVector(pointer data, size_type sz, size_type cap)
            : m_data(data)
            , m_size(sz)
            , m_cap(cap) { }


public:  // Constructors
    UniquePtrVector()
            : m_data(new value_type[1])
            , m_size(0)
            , m_cap(1) {
        m_data[0] = nullptr;
    }

    explicit UniquePtrVector(size_type sz)
            : m_data(new value_type[sz])
            , m_size(0)
            , m_cap(sz) {
        std::fill(m_data, m_data + sz, nullptr);
    }

    UniquePtrVector(const UniquePtrVector &) = delete;
    UniquePtrVector &operator=(const UniquePtrVector &) = delete;

    UniquePtrVector(UniquePtrVector &&) {
        unreachable();
    }
    UniquePtrVector &operator=(UniquePtrVector &&) {
        unreachable();
        return *this;
    }

    ~UniquePtrVector() {
        if (!m_data)
            return;
        for (size_type i = 0; i < m_size; i++)
            delete m_data[i];
        delete[] m_data;
    }


private:  // Memebr funcs
    static constexpr size_type expandFactor(size_type cap) {
        return cap * 2;
    }

    bool maybeExpand(size_type sz = 1) {
        if ((m_size + sz) <= m_cap)
            return false;

        m_cap = expandFactor(m_cap);
        pointer tmp = new value_type[m_cap];
        std::memcpy(tmp, m_data, sizeof(value_type) * m_size);
        std::swap(tmp, m_data);
        delete[] tmp;
        return true;
    }

    void throwIfOutOfRange(size_type index) {
        if (index >= m_size)
            throw std::range_error("index is greater than the size of the container");
        if (index < 0)
            throw std::range_error("index less than 0");
    }

public:  // Member funcs
    void pushBack(value_type ptr) {
        maybeExpand();
        m_data[m_size++] = ptr;
    }

    void pushBack(unique_value_type &&ptr) {
        maybeExpand();
        m_data[m_size++] = ptr.release();
    }

    const_value_type operator[](size_type index) const noexcept {
        return m_data[index];
    }

    reference operator[](size_type index) noexcept {
        return m_data[index];
    }

    const_value_type at(size_type index) const {
        throwIfOutOfRange(index);
        return m_data[index];
    }

    reference at(size_type index) {
        throwIfOutOfRange(index);
        return m_data[index];
    }

    const_value_type front() const noexcept {
        return m_data[0];
    }

    reference front() noexcept {
        return m_data[0];
    }

    const_value_type back() const noexcept {
        return m_data[m_size - 1];
    }

    reference back() noexcept {
        return m_data[m_size - 1];
    }



public:  // Factories
    template<typename... Ts>
    static UniquePtrVector fromPtr(Ts... args) requires(dktt::check_all_v<std::is_pointer, Ts...>) {
        pointer data = new value_type[sizeof...(Ts)];
        size_type sz(sizeof...(Ts));
        size_type cap(sizeof...(Ts));
        value_type tmp[] = {args...};
        std::copy(std::begin(tmp), std::end(tmp), data);
        return UniquePtrVector {data, sz, cap};
    }


    template<typename... Ts>
    static UniquePtrVector fromPtr(Ts &&...args) requires(dktt::check_all_v<is_unique_ptr, Ts...>) {
        pointer data = new value_type[sizeof...(Ts)];
        size_type sz(sizeof...(Ts));
        size_type cap(sizeof...(Ts));
        unique_value_type tmp[] = {std::move(args)...};
        std::transform(std::begin(tmp), std::end(tmp), data, [](auto &&ptr) { return ptr.release(); });
        return UniquePtrVector {data, sz, cap};
    }
};
