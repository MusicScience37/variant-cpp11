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

TEST_CASE("variant_cpp11::impl::max_size") {
    using variant_cpp11::impl::max_size;

    SECTION("no value") {
        constexpr std::size_t res = max_size<>::get();
        REQUIRE(res == 0);
    }

    SECTION("one value") {
        constexpr std::size_t res = max_size<3>::get();
        REQUIRE(res == 3);
    }

    SECTION("two values") {
        constexpr std::size_t res = max_size<2, 5>::get();
        REQUIRE(res == 5);
    }

    SECTION("three values") {
        constexpr std::size_t res = max_size<4, 7, 2>::get();
        REQUIRE(res == 7);
    }

    SECTION("four values") {
        constexpr std::size_t res = max_size<2, 10, 37, 5>::get();
        REQUIRE(res == 37);
    }
}
