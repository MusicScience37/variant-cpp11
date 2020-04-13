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
#include <utility>

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
inline constexpr std::size_t invalid_index() {
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

    /*!
     * \brief get void pointer of data
     *
     * \return void* void pointer of data
     */
    void* void_ptr() noexcept { return static_cast<void*>(data); }
};

/*!
 * \brief create a object
 *
 * \tparam creating_type type of object to create
 * \tparam arg_types types of args
 * \param ptr pointer in which the object is created
 * \param args arguments of constructor
 */
template <typename creating_type, typename... arg_types>
inline void create(void* ptr, arg_types&&... args) {
    ::new (ptr) creating_type(std::forward<arg_types>(args)...);
}

/*!
 * \brief helper struct for variant
 *
 * \tparam front_index index of the first type in template parameters
 * \tparam types types in variant
 */
template <std::size_t front_index, typename... types>
struct variant_helper;

/*!
 * \brief variant_helper for multiple types
 *
 * \tparam front_index index of front_type
 * \tparam front_type first type
 * \tparam remaining_types remaining types
 */
template <std::size_t front_index, typename front_type,
    typename... remaining_types>
struct variant_helper<front_index, front_type, remaining_types...> {
    //! variant_helper of remaining types
    using remaining_helper =
        variant_helper<front_index + 1, remaining_types...>;

    /*!
     * \brief get index of a type
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t type_index() {
        return (std::is_same<type, front_type>::value)
            ? front_index
            : remaining_helper::template type_index<type>();
    }

    /*!
     * \brief get index of a type similar to given type
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t similar_type_index() {
        return (std::is_same<typename std::decay<type>::type,
                   typename std::decay<front_type>::type>::value)
            ? front_index
            : remaining_helper::template similar_type_index<type>();
    }

    /*!
     * \brief get index of a type convertible from given type
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t convertible_type_index() {
        return (std::is_convertible<type, front_type>::value)
            ? front_index
            : remaining_helper::template convertible_type_index<type>();
    }

    /*!
     * \brief get index of a type which can be assigned
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t assigning_type_index() {
        return (type_index<type>() != invalid_index())
            ? type_index<type>()
            : (similar_type_index<type>() != invalid_index())
                ? similar_type_index<type>()
                : convertible_type_index<type>();
    }

    /*!
     * \brief type of an index
     *
     * \tparam index index
     */
    template <std::size_t index>
    using index_type =
        typename std::conditional<index == front_index, front_type,
            typename remaining_helper::template index_type<index>>::type;

    /*!
     * \brief destoy the object in a pointer
     *
     * \param index index of type
     * \param ptr pointer
     */
    static void destroy(std::size_t index, void* ptr) noexcept {
        if (index == front_index) {
            static_cast<front_type*>(ptr)->~front_type();
        } else {
            remaining_helper::destroy(index, ptr);
        }
    }
};

/*!
 * \brief variant_helper for no type
 *
 * \tparam front_index index
 */
template <std::size_t front_index>
struct variant_helper<front_index> {
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

    /*!
     * \brief get index of a type similar to given type
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t similar_type_index() {
        return invalid_index();
    }

    /*!
     * \brief get index of a type convertible from given type
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t convertible_type_index() {
        return invalid_index();
    }

    /*!
     * \brief get index of a type which can be assigned
     *
     * \tparam type type
     * \return constexpr std::size_t index
     */
    template <typename type>
    static constexpr std::size_t assigning_type_index() {
        return invalid_index();
    }

    /*!
     * \brief type of an index
     *
     * \tparam index index
     */
    template <std::size_t index>
    using index_type = invalid_type;

    /*!
     * \brief destoy the object in a pointer
     *
     * \param index index of type
     * \param ptr pointer
     */
    static void destroy(std::size_t index, void* ptr) noexcept {}
};

}  // namespace impl

template <typename... stored_types>
class variant {
private:
    //! helper type
    using helper = impl::variant_helper<0, stored_types...>;

    //! storage type
    using storage_type = impl::variant_storage<stored_types...>;

    //! storage
    storage_type _storage;

    //! type index
    std::size_t _index;

public:
    //! default constructor (create invalid object)
    variant() noexcept : _storage(), _index(invalid_index()) {}

    /*!
     * \brief construct with an object
     *
     * \tparam type type of the object to create in this object
     * \param obj object to copy or move from
     */
    template <typename type>
    // NOLINTNEXTLINE(hicpp-member-init,google-explicit-constructor,hicpp-explicit-conversions)
    variant(type&& obj) : variant() {
        constexpr std::size_t type_index =
            helper::template assigning_type_index<type>();
        emplace<typename helper::template index_type<type_index>>(
            std::forward<type>(obj));
    }

    //! \todo implementation
    variant(const variant&) = delete;
    //! \todo implementation
    variant(variant&&) = delete;
    //! \todo implementation
    variant& operator=(const variant&) = delete;
    //! \todo implementation
    variant& operator=(variant&&) = delete;

    /*!
     * \brief destroy the object
     */
    ~variant() { destroy(); }

    /*!
     * \brief create an object in this object
     *
     * \tparam created_type type of the object to create
     * \tparam arg_types types of args
     * \param args arguments of constructor
     * \return created_type& created object
     */
    template <typename created_type, typename... arg_types>
    created_type& emplace(arg_types&&... args) {
        constexpr std::size_t index =
            helper::template type_index<created_type>();
        static_assert(index != invalid_index(), "invalid type");

        destroy();
        // next line may throw an exception
        impl::create<created_type, arg_types...>(
            void_ptr(), std::forward<arg_types>(args)...);
        _index = index;
        return get_no_check<created_type>();
    }

private:
    /*!
     * \brief get void pointer of data
     *
     * \return void* void pointer
     */
    void* void_ptr() noexcept { return _storage.void_ptr(); }

    /*!
     * \brief destroy the object
     */
    void destroy() noexcept { helper::destroy(_index, void_ptr()); }

    /*!
     * \brief get object without check
     *
     * \tparam type type to get
     * \return type& object
     */
    template <typename type>
    type& get_no_check() {
        return *static_cast<type*>(void_ptr());
    }
};

}  // namespace variant_cpp11
