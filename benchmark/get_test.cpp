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
#include <benchmark/benchmark.h>

#include "variant_cpp11/variant.h"

static constexpr std::size_t vector_size = 100000;

static void get_int_from_union(benchmark::State& state) {
    union test_type {
        int val_int;
        float val_float;
    };

    test_type test;
    test.val_int = 5;
    std::vector<test_type> vec(vector_size, test);

    for (auto _ : state) {
        for (auto& elem : vec) {
            int val = elem.val_int;
            benchmark::DoNotOptimize(val);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(get_int_from_union);

static void get_int_from_variant(benchmark::State& state) {
    using test_type = variant_cpp11::variant<int, float>;

    std::vector<test_type> vec(vector_size, test_type(5));

    for (auto _ : state) {
        for (auto& elem : vec) {
            int val = elem.get<int>();
            benchmark::DoNotOptimize(val);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(get_int_from_variant);

static void get_string_from_variant(benchmark::State& state) {
    using test_type = variant_cpp11::variant<int, float, std::string>;

    std::vector<test_type> vec(vector_size, test_type("abc"));

    for (auto _ : state) {
        for (auto& elem : vec) {
            std::string& val = elem.get<std::string>();
            benchmark::DoNotOptimize(val);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(get_string_from_variant);
