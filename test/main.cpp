#include <catch2/catch_test_macros.hpp>  // for StringRef, operator""_catch_sr
#include <cstddef>                       // for nullptr_t
#include <scc/scc.hpp>                   // for ValuePtr
#include <utility>                       // for move

// Has to be a macro
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define REQUIRE_NOTHROW_BLOCK(B) REQUIRE_NOTHROW([]() B())


struct NoDefualtCtor {
    NoDefualtCtor() = delete;
    explicit NoDefualtCtor(int /*unused*/) { }
};

struct NoCopy {
    NoCopy() = default;

    NoCopy(const NoCopy &) = delete;
    NoCopy &operator=(const NoCopy &) = delete;

    NoCopy(NoCopy &&) = default;
    NoCopy &operator=(NoCopy &&) = default;

    ~NoCopy() = default;
};

struct NoCopyDerived : public NoCopy { };

struct NoMove {
    NoMove() = default;

    NoMove(const NoMove &) = default;
    NoMove &operator=(const NoMove &) = default;

    NoMove(NoMove &&) = delete;
    NoMove &operator=(NoMove &&) = delete;

    ~NoMove() = default;
};


struct NoMoveDerived : public NoMove { };

struct Base { };
struct Derived : public Base { };


TEST_CASE("default_ctor", "[ValuePtr]") {
    // Should be default constructible with default constructible types
    REQUIRE(std::constructible_from<ValuePtr<Base>>);
    REQUIRE_NOTHROW_BLOCK({ ValuePtr<Base> b {}; });

    // Should not be default constructible with not default constructible types
    REQUIRE(!std::constructible_from<ValuePtr<NoDefualtCtor>>);
}

TEST_CASE("nullptr_ctor", "[ValuePtr]") {
    // Should not be default constructible from nullptr
    REQUIRE(!std::constructible_from<ValuePtr<Base>, std::nullptr_t>);
}


TEST_CASE("pointer_ctor", "[ValuePtr]") {
    REQUIRE(std::constructible_from<ValuePtr<Base>, Base *>);
    REQUIRE_NOTHROW_BLOCK({ ValuePtr<Base> b {new Base()}; });

    REQUIRE(std::constructible_from<ValuePtr<NoDefualtCtor>, NoDefualtCtor *>);
    REQUIRE_NOTHROW_BLOCK({ ValuePtr<NoDefualtCtor> b {new NoDefualtCtor(42)}; });
}

TEST_CASE("copy_ctor", "[ValuePtr]") {
    REQUIRE(std::constructible_from<ValuePtr<Base>, ValuePtr<Base>>);

    REQUIRE_NOTHROW_BLOCK({
        [[maybe_unused]] ValuePtr<Base> a;
        [[maybe_unused]] ValuePtr<Base> b = a;  // NOLINT(performance-unnecessary-copy-initialization)
    });
    REQUIRE(!std::constructible_from<ValuePtr<NoCopy>, ValuePtr<NoCopy> &>);
}

TEST_CASE("move_ctor", "[ValuePtr]") {
    REQUIRE(std::constructible_from<ValuePtr<Base>, ValuePtr<Base>>);

    REQUIRE_NOTHROW_BLOCK({
        [[maybe_unused]] ValuePtr<Base> a;
        [[maybe_unused]] ValuePtr<Base> b = std::move(a);
    });

    REQUIRE(!std::constructible_from<ValuePtr<NoMove>, ValuePtr<NoMove &&>>);
}

TEST_CASE("derived_pointer_ctor", "[ValuePtr]") {
    REQUIRE(std::constructible_from<ValuePtr<Base>, Derived *>);
    REQUIRE_NOTHROW_BLOCK({ ValuePtr<Base> b {new Derived()}; });
}

TEST_CASE("derived_copy_ctor", "[ValuePtr]") {
    REQUIRE(std::constructible_from<ValuePtr<Base>, ValuePtr<Derived>>);

    REQUIRE_NOTHROW_BLOCK({
        [[maybe_unused]] ValuePtr<Derived> a;
        [[maybe_unused]] ValuePtr<Base> b = a;  // NOLINT(performance-unnecessary-copy-initialization)
    });
    REQUIRE(!std::constructible_from<ValuePtr<NoCopy>, ValuePtr<NoCopyDerived> &>);
}

TEST_CASE("derived_move_ctor", "[ValuePtr]") {
    REQUIRE(std::constructible_from<ValuePtr<Base>, ValuePtr<Derived>>);

    REQUIRE_NOTHROW_BLOCK({
        [[maybe_unused]] ValuePtr<Derived> a;
        [[maybe_unused]] ValuePtr<Base> b = std::move(a);
    });

    REQUIRE(!std::constructible_from<ValuePtr<NoMove>, ValuePtr<NoMoveDerived &&>>);
}

TEST_CASE("nullptr_assign", "[ValuePtr]") {
    REQUIRE(!std::assignable_from<ValuePtr<Base>, std::nullptr_t>);
}

TEST_CASE("copy_assign", "[ValuePtr]") {
    REQUIRE_NOTHROW_BLOCK({
        [[maybe_unused]] ValuePtr<Base> a;
        [[maybe_unused]] ValuePtr<Base> b;
        a = b;
    });
}

TEST_CASE("move_assign", "[ValuePtr]") {
    REQUIRE_NOTHROW_BLOCK({
        [[maybe_unused]] ValuePtr<Base> a;
        [[maybe_unused]] ValuePtr<Base> b;
        a = std::move(b);
    });
}

TEST_CASE("derived_copy_assign", "[ValuePtr]") {
    REQUIRE(!std::assignable_from<ValuePtr<Base>, ValuePtr<Derived>>);
}

TEST_CASE("derived_move_assign", "[ValuePtr]") {
    REQUIRE_NOTHROW_BLOCK({
        [[maybe_unused]] ValuePtr<Base> a;
        [[maybe_unused]] ValuePtr<Derived> b;
        a = std::move(b);
    });
}
