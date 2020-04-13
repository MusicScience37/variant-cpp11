/*
 * MIT License
 *
 * Copyright (c) 2020 MusicScience37 (Kenta Kabashima)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <catch2/catch.hpp>

#include "variant_cpp11/variant.h"

namespace {

struct object_count {
    static int count;

    object_count() noexcept { ++count; }
    explicit object_count(int /*unused*/) noexcept { ++count; }

    object_count(const object_count& /*unused*/) noexcept { ++count; }
    object_count(object_count&& /*unused*/) noexcept { ++count; }

    // NOLINTNEXTLINE(cert-oop54-cpp)
    object_count& operator=(const object_count& /*unused*/) noexcept {
        ++count;
        return *this;
    }
    object_count& operator=(object_count&& /*unused*/) noexcept {
        ++count;
        return *this;
    }

    ~object_count() { --count; }
};

int object_count::count = 0;

}  // namespace

TEST_CASE("variant_cpp11::impl::variant_helper") {
    using variant_cpp11::impl::create;
    using variant_cpp11::impl::variant_helper;
    using variant_cpp11::impl::variant_storage;

    SECTION("type_index function") {
        using test_type = variant_helper<0, int, float, double>;
        constexpr std::size_t index_int = test_type::type_index<int>();
        REQUIRE(index_int == 0);
        constexpr std::size_t index_float = test_type::type_index<float>();
        REQUIRE(index_float == 1);
        constexpr std::size_t index_double = test_type::type_index<double>();
        REQUIRE(index_double == 2);
        constexpr std::size_t index_const_int =
            test_type::type_index<const int>();
        REQUIRE(index_const_int == variant_cpp11::invalid_index());
    }

    SECTION("index_type alias") {
        using test_type = variant_helper<0, float, double>;
        using type0 = typename test_type::index_type<0>;
        REQUIRE(std::is_same<type0, float>::value == true);
        using type1 = typename test_type::index_type<1>;
        REQUIRE(std::is_same<type1, double>::value == true);
        using type2 = typename test_type::index_type<2>;
        REQUIRE(
            std::is_same<type2, variant_cpp11::invalid_type>::value == true);
    }

    SECTION("assign and destroy") {
        using test_helper = variant_helper<0, float, object_count, int>;
        using test_storage = variant_storage<float, object_count, int>;
        test_storage storage;

        REQUIRE(object_count::count == 0);
        create<object_count>(storage.void_ptr());
        REQUIRE(object_count::count == 1);
        test_helper::destroy(1, storage.void_ptr());
        REQUIRE(object_count::count == 0);

        REQUIRE(object_count::count == 0);
        create<object_count>(storage.void_ptr(), 3);
        REQUIRE(object_count::count == 1);
        test_helper::destroy(1, storage.void_ptr());
        REQUIRE(object_count::count == 0);
    }
}
