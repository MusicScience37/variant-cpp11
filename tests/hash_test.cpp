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
#include <unordered_map>

#include "variant_cpp11/variant.h"

TEST_CASE("std::hash<variant_cpp11::variant>") {
    SECTION("calculate hash number") {
        using variant_type = variant_cpp11::variant<int, float, std::string>;
        using hash_type = std::hash<variant_type>;
        hash_type hash;
        variant_type obj;

        REQUIRE(obj.has_value() == false);
        REQUIRE(hash(obj) == variant_cpp11::invalid_index());

        obj = 3;
        REQUIRE(obj.has<int>() == true);
        REQUIRE(hash(obj) == 0 + std::hash<int>()(3));

        obj = 1.0F;
        REQUIRE(obj.has<float>() == true);
        REQUIRE(hash(obj) == 1 + std::hash<float>()(1.0F));

        obj = "abc";
        REQUIRE(obj.has<std::string>() == true);
        REQUIRE(hash(obj) == 2 + std::hash<std::string>()("abc"));
    }

    SECTION("use in unordered_map") {
        using variant_type = variant_cpp11::variant<int, int, std::string>;

        std::unordered_map<variant_type, int> map;
        REQUIRE_NOTHROW(map.emplace(1, 2));
        variant_type object;
        object.emplace<1>(1);
        REQUIRE_NOTHROW(map.emplace(object, 3));
        REQUIRE_NOTHROW(map.emplace("abc", 4));

        REQUIRE(map.at(1) == 2);
        REQUIRE(map.at(object) == 3);
        REQUIRE(map.at("abc") == 4);
    }
}
