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
#include "variant_cpp11/variant.h"

#include <catch2/catch.hpp>
#include <memory>

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

TEST_CASE("variant_cpp11::variant") {
    SECTION("no type") {
        std::shared_ptr<variant_cpp11::variant<>> ptr;
        REQUIRE_NOTHROW(ptr = std::make_shared<variant_cpp11::variant<>>());
        REQUIRE_NOTHROW(ptr.reset());
    }

    SECTION("construct and destruct with one type") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE(object_count::count == 0);
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>());
        REQUIRE(object_count::count == 0);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 0);
    }

    SECTION("copy construct from an object and destruct with one type") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE(object_count::count == 0);
        object_count obj;
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(obj));
        REQUIRE(object_count::count == 2);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("emplace and destruct with one type") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE(object_count::count == 0);
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>());
        REQUIRE(object_count::count == 0);
        REQUIRE_NOTHROW(ptr->emplace<object_count>());
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 0);
    }
}
