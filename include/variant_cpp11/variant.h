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
#include <limits>
#include <type_traits>

/*!
 * \brief namespace of variant-cpp11 project
 */
namespace variant_cpp11 {

/*
 * Definitions
 */

/*!
 * \brief get index for invalid types
 *
 * \return constexpr std::size_t index
 */
constexpr std::size_t invalid_index() {
    return std::numeric_limits<std::size_t>::max();
}

struct invalid_type {};

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

/*!
 * \brief helper struct for variant
 *
 * \tparam index index of the first type in template parameters
 * \tparam types types in variant
 */
template <std::size_t index, typename... types>
struct variant_helper;

/*!
 * \brief variant_helper for multiple types
 *
 * \tparam index index
 * \tparam front_type first type
 * \tparam remaining_types remaining types
 */
template <std::size_t index, typename front_type, typename... remaining_types>
struct variant_helper<index, front_type, remaining_types...> {
    //! variant_helper of remaining types
    using remaining_helper = variant_helper<index + 1, remaining_types...>;

    /*!
     * \brief get index of a type
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t type_index() {
        return (std::is_same<type, front_type>::value)
            ? index
            : remaining_helper::template type_index<type>();
    }

    //! type of an index
    template <std::size_t index_to_get>
    using index_type =
        typename std::conditional<index_to_get == index, front_type,
            typename remaining_helper::template index_type<index_to_get>>::type;
};

/*!
 * \brief variant_helper for no type
 *
 * \tparam index index
 */
template <std::size_t index>
struct variant_helper<index> {
    /*!
     * \brief get index of a type
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t type_index() {
        return invalid_index();
    }

    template <std::size_t index_to_get>
    using index_type = invalid_type;
};

}  // namespace impl

}  // namespace variant_cpp11
