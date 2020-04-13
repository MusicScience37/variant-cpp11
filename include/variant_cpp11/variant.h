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
#pragma once

#include <cstddef>

/*!
 * \brief namespace of variant-cpp11 project
 */
namespace variant_cpp11 {

/*!
 * \brief namespace of implementation details
 *
 * \warning API in this namespace may be unstable.
 */
namespace impl {

/*!
 * \brief template struct to get maximum std::size_t value in template
 * parameters
 *
 * \tparam values values whose max is calculated
 */
template <std::size_t... values>
struct max_size;

/*!
 * \brief max_size for multiple values
 *
 * \tparam front_val first value in the template parameters
 * \tparam remaining_val remaining values in the template parameters
 */
template <std::size_t front_val, std::size_t... remaining_val>
struct max_size<front_val, remaining_val...> {
    //! max_size of remaining_val
    using remaining_type = max_size<remaining_val...>;

    /*!
     * \brief get maximum value of all the template parameters
     *
     * \return constexpr std::size_t maximum value
     */
    static constexpr std::size_t get() {
        return (front_val > remaining_type::get()) ? front_val
                                                   : remaining_type::get();
    }
};

/*!
 * \brief max_size for no value
 */
template <>
struct max_size<> {
    /*!
     * \brief get maximum value of all the template parameters
     *
     * \return constexpr std::size_t maximum value
     */
    static constexpr std::size_t get() { return 0; }
};

/*!
 * \brief storage for variant
 *
 * \tparam types types used in variant
 */
template <typename... types>
struct variant_storage {
    /*!
     * \brief get size of the storage
     *
     * \return constexpr std::size_t size of the storage
     */
    static constexpr std::size_t get_size() {
        return max_size<sizeof(types)...>::get();
    }

    /*!
     * \brief get alignment of the storage
     *
     * \return constexpr std::size_t alignment of the storage
     */
    static constexpr std::size_t get_align() {
        return max_size<alignof(types)...>::get();
    }

    //! storage
    alignas(get_align()) char data[get_size()];
};

}  // namespace impl

}  // namespace variant_cpp11
