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
#include <string>

#include "object_count.h"

TEST_CASE("variant_cpp11::variant") {
    object_count::count = 0;

    SECTION("no type") {
        std::shared_ptr<variant_cpp11::variant<>> ptr;
        REQUIRE_NOTHROW(ptr = std::make_shared<variant_cpp11::variant<>>());
        REQUIRE(ptr->index() == variant_cpp11::invalid_index());
        REQUIRE_NOTHROW(ptr.reset());
    }

    SECTION("construct and destruct with one type") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE(object_count::count == 0);
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>());
        REQUIRE(object_count::count == 0);
        REQUIRE(ptr->index() == variant_cpp11::invalid_index());
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
        REQUIRE(ptr->index() == 0);
        REQUIRE(object_count::count == 2);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("move construct from an object and destruct with one type") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE(object_count::count == 0);
        object_count obj;
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(
                std::move(obj)));
        REQUIRE(object_count::count == 1);
        REQUIRE(ptr->index() == 0);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 0);
    }

    SECTION("emplace and destruct with one type") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE(object_count::count == 0);
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>());
        REQUIRE(object_count::count == 0);
        REQUIRE_NOTHROW(ptr->emplace<object_count>());
        REQUIRE(object_count::count == 1);
        REQUIRE(ptr->index() == 0);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 0);
    }

    SECTION("copy constructor") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        object_count obj;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(obj));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        // copy construct
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr_copy;
        REQUIRE_NOTHROW(
            ptr_copy =
                std::make_shared<variant_cpp11::variant<object_count>>(*ptr));
        REQUIRE(object_count::count == 3);
        REQUIRE(ptr_copy->index() == 0);

        // destruct
        REQUIRE_NOTHROW(ptr_copy.reset());
        REQUIRE(object_count::count == 2);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("copy assignment") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        object_count obj;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(obj));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        std::shared_ptr<variant_cpp11::variant<object_count>> ptr_copy;
        REQUIRE_NOTHROW(
            ptr_copy =
                std::make_shared<variant_cpp11::variant<object_count>>());

        // copy assignment
        REQUIRE_NOTHROW(*ptr_copy = *ptr);
        REQUIRE(object_count::count == 3);
        REQUIRE(ptr_copy->index() == 0);

        // destruct
        REQUIRE_NOTHROW(ptr_copy.reset());
        REQUIRE(object_count::count == 2);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("self copy assignment") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        object_count obj;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(obj));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        // copy assignment
        REQUIRE_NOTHROW(*ptr = *ptr);
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        // destruct
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("move constructor") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        object_count obj;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(obj));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        // move construct
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr_copy;
        REQUIRE_NOTHROW(
            ptr_copy = std::make_shared<variant_cpp11::variant<object_count>>(
                std::move(*ptr)));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr_copy->index() == 0);
        REQUIRE(ptr->index() == variant_cpp11::invalid_index());

        // destruct
        REQUIRE_NOTHROW(ptr_copy.reset());
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("move assignment") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        object_count obj;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(obj));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        std::shared_ptr<variant_cpp11::variant<object_count>> ptr_copy;
        REQUIRE_NOTHROW(
            ptr_copy =
                std::make_shared<variant_cpp11::variant<object_count>>());

        // move assignment
        REQUIRE_NOTHROW(*ptr_copy = std::move(*ptr));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr_copy->index() == 0);
        REQUIRE(ptr->index() == variant_cpp11::invalid_index());

        // destruct
        REQUIRE_NOTHROW(ptr_copy.reset());
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("self move assignment") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        object_count obj;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>(obj));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        // move assignment
        REQUIRE_NOTHROW(*ptr = std::move(*ptr));
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);

        // destruct
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 1);
    }

    SECTION("assignment operator with one type") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>());
        REQUIRE(object_count::count == 0);
        object_count obj;
        REQUIRE(object_count::count == 1);
        REQUIRE_NOTHROW(*ptr = obj);
        REQUIRE(object_count::count == 2);
        REQUIRE(ptr->index() == 0);
        REQUIRE_NOTHROW(*ptr = std::move(obj));
        REQUIRE(object_count::count == 1);
        REQUIRE(ptr->index() == 0);
        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 0);
    }

    SECTION("emplace and destruct with multiple types") {
        using test_type =
            variant_cpp11::variant<int, std::string, object_count>;
        std::shared_ptr<test_type> ptr;
        REQUIRE_NOTHROW(ptr = std::make_shared<test_type>());

        REQUIRE(ptr->emplace<int>(5) == 5);
        REQUIRE(object_count::count == 0);
        REQUIRE(ptr->index() == 0);

        REQUIRE_NOTHROW(ptr->emplace<object_count>());
        REQUIRE(object_count::count == 1);
        REQUIRE(ptr->index() == 2);

        REQUIRE(ptr->emplace<std::string>("abc") == "abc");
        REQUIRE(object_count::count == 0);
        REQUIRE(ptr->index() == 1);

        REQUIRE_NOTHROW(ptr->emplace<2>());
        REQUIRE(object_count::count == 1);
        REQUIRE(ptr->index() == 2);

        REQUIRE(ptr->emplace<0>(37) == 37);
        REQUIRE(object_count::count == 0);
        REQUIRE(ptr->index() == 0);

        REQUIRE_NOTHROW(ptr.reset());
        REQUIRE(object_count::count == 0);
    }

    SECTION("get value") {
        using test_type = variant_cpp11::variant<int, std::string>;
        std::shared_ptr<test_type> ptr;
        REQUIRE_NOTHROW(ptr = std::make_shared<test_type>());
        std::shared_ptr<const test_type> const_ptr = ptr;

        REQUIRE(ptr->emplace<int>(5) == 5);
        REQUIRE(ptr->get<0>() == 5);
        REQUIRE(const_ptr->get<0>() == 5);
        REQUIRE_THROWS(ptr->get<1>());
        REQUIRE_THROWS(const_ptr->get<1>());

        REQUIRE(ptr->get<int>() == 5);
        REQUIRE(const_ptr->get<int>() == 5);
        REQUIRE_THROWS(ptr->get<std::string>());
        REQUIRE_THROWS(const_ptr->get<std::string>());
    }

    SECTION("selection of correct types") {
        using test_type = variant_cpp11::variant<std::string, char*>;
        std::shared_ptr<test_type> ptr;
        char char_array[] = "abc";
        REQUIRE_NOTHROW(ptr = std::make_shared<test_type>(char_array));
        REQUIRE(ptr->index() == 1);
        const char const_char_array[] = "abc";
        REQUIRE_NOTHROW(ptr = std::make_shared<test_type>(const_char_array));
        REQUIRE(ptr->index() == 0);
    }

    SECTION("use of multiple same types") {
        using test_type = variant_cpp11::variant<int, int>;
        std::shared_ptr<test_type> ptr;
        REQUIRE_NOTHROW(ptr = std::make_shared<test_type>());

        REQUIRE(ptr->emplace<int>(5) == 5);
        REQUIRE(ptr->index() == 0);

        REQUIRE(ptr->emplace<0>(37) == 37);
        REQUIRE(ptr->index() == 0);

        REQUIRE(ptr->emplace<1>(37) == 37);
        REQUIRE(ptr->index() == 1);
    }

    SECTION("has_value function") {
        std::shared_ptr<variant_cpp11::variant<object_count>> ptr;
        REQUIRE_NOTHROW(
            ptr = std::make_shared<variant_cpp11::variant<object_count>>());
        REQUIRE(ptr->has_value() == false);
        REQUIRE_NOTHROW(ptr->emplace<object_count>());
        REQUIRE(ptr->has_value() == true);

        if (*ptr) {
            SUCCEED();
        } else {
            FAIL();
        }

        if (!*ptr) {
            FAIL();
        } else {
            SUCCEED();
        }
    }

    SECTION("has function") {
        using test_type = variant_cpp11::variant<int, std::string>;
        test_type object(5);

        REQUIRE(object.has<int>() == true);
        REQUIRE(object.has<std::string>() == false);

        object = "abc";

        REQUIRE(object.has<int>() == false);
        REQUIRE(object.has<std::string>() == true);
    }
}
