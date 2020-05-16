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
#include <cstdint>

#include "object_count.h"
#include "variant_cpp11/variant.h"

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

    SECTION("similar_type_index function") {
        using test_type = variant_helper<0, int, float, double>;
        constexpr std::size_t index_const_int =
            test_type::similar_type_index<const int>();
        REQUIRE(index_const_int == 0);
        constexpr std::size_t index_const_int_pointer =
            test_type::similar_type_index<const int*>();
        REQUIRE(index_const_int_pointer == variant_cpp11::invalid_index());
    }

    SECTION("convertible_type_index function") {
        using test_type = variant_helper<0, float, std::string>;
        constexpr std::size_t index_char_pointer =
            test_type::convertible_type_index<char*>();
        REQUIRE(index_char_pointer == 1);
        constexpr std::size_t index_short =
            test_type::convertible_type_index<short>();  // NOLINT
        REQUIRE(index_short == 0);
        constexpr std::size_t index_int_pointer =
            test_type::convertible_type_index<int*>();
        REQUIRE(index_int_pointer == variant_cpp11::invalid_index());
    }

    SECTION("assigning_type_index function") {
        using test_type =
            variant_helper<0, int, float, std::string, object_count>;
        constexpr std::size_t index_int =
            test_type::assigning_type_index<int>();
        REQUIRE(index_int == 0);
        constexpr std::size_t index_float =
            test_type::assigning_type_index<float>();
        REQUIRE(index_float == 1);
        constexpr std::size_t index_string =
            test_type::assigning_type_index<std::string>();
        REQUIRE(index_string == 2);

        constexpr std::size_t index_const_float =
            test_type::assigning_type_index<const float>();
        REQUIRE(index_const_float == 1);
        constexpr std::size_t index_const_object_count_ref =
            test_type::assigning_type_index<const object_count&>();
        REQUIRE(index_const_object_count_ref == 3);

        constexpr std::size_t index_char =
            test_type::assigning_type_index<char>();
        REQUIRE(index_char == 0);
        constexpr std::size_t index_const_char_pointer =
            test_type::assigning_type_index<const char*>();
        REQUIRE(index_const_char_pointer == 2);
        constexpr std::size_t index_int_pointer =
            test_type::assigning_type_index<int*>();
        REQUIRE(index_int_pointer == variant_cpp11::invalid_index());
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

    SECTION("copy") {
        using test_helper = variant_helper<0, float, object_count, int>;
        using test_storage = variant_storage<float, object_count, int>;
        test_storage from;
        test_storage to;

        create<object_count>(from.void_ptr());
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(test_helper::copy(1, from.void_ptr(), to.void_ptr()));
        REQUIRE(object_count::count == 2);
        test_helper::destroy(1, from.void_ptr());
        test_helper::destroy(1, to.void_ptr());
        REQUIRE(object_count::count == 0);
    }

    SECTION("move") {
        using test_helper = variant_helper<0, float, object_count, int>;
        using test_storage = variant_storage<float, object_count, int>;
        test_storage from;
        test_storage to;

        create<object_count>(from.void_ptr());
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(test_helper::move(1, from.void_ptr(), to.void_ptr()));
        REQUIRE(object_count::count == 1);
        test_helper::destroy(1, to.void_ptr());
        REQUIRE(object_count::count == 0);
    }

    SECTION("is_all_nothrow_movable") {
        struct test_type {             // NOLINT
            test_type(test_type&&) {}  // NOLINT
        };
        STATIC_REQUIRE(
            variant_helper<0, int, float>::is_all_nothrow_movable == true);
        STATIC_REQUIRE(
            variant_helper<0, int, test_type>::is_all_nothrow_movable == false);
    }

    SECTION("equal") {
        using test_helper = variant_helper<0, float, object_count, int>;
        using test_storage = variant_storage<float, object_count, int>;
        test_storage left;
        test_storage right;

        create<int>(left.void_ptr(), 3);
        create<int>(right.void_ptr(), 3);
        REQUIRE(
            test_helper::equal(2, left.void_ptr(), right.void_ptr()) == true);
        create<int>(right.void_ptr(), 4);
        REQUIRE(
            test_helper::equal(2, left.void_ptr(), right.void_ptr()) == false);
        test_helper::destroy(2, left.void_ptr());
        test_helper::destroy(2, right.void_ptr());

        create<object_count>(left.void_ptr());
        create<object_count>(right.void_ptr());
        REQUIRE_THROWS(
            test_helper::equal(1, left.void_ptr(), right.void_ptr()));
        test_helper::destroy(1, left.void_ptr());
        test_helper::destroy(1, right.void_ptr());

        REQUIRE(
            test_helper::equal(3, left.void_ptr(), right.void_ptr()) == true);
    }
}
