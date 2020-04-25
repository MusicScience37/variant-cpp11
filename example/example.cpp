#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

#include "variant_cpp11/variant.h"

/*!
 * \brief namespace for examples of variant
 */
namespace variant_cpp11_examples {

/*!
 * \brief check whether result is true
 *
 * \param result result of comparisons
 */
void check(bool result) {
    if (!result) {
        std::cout << "\nCHECK FAILED!!\n" << std::endl;
        std::exit(1);
    }
}

/*!
 * \brief example of assigning values
 */
void assign() {
    std::cout << "# Test of assigning values\n" << std::endl;

    // default constructor will create an object without value
    variant_cpp11::variant<int, float, std::string> obj;

    // copy a value
    obj = 123;

    // move a value
    obj = std::move(std::string("abc"));

    // destructor will destroy stored values
};

/*!
 * \brief example of getting values
 */
void get() {
    std::cout << "# Test of getting values\n" << std::endl;

    variant_cpp11::variant<float, std::string> obj("abc");

    // get value
    std::cout << "- string: " << obj.get<std::string>() << std::endl;
    check(obj.get<std::string>() == "abc");

    // wrong type will throw an exception
    try {
        const float val = obj.get<float>();  // this throws
        std::cout << "- wrongly got value: " << val << std::endl;
        check(false);
    } catch (variant_cpp11::variant_error& e) {
        std::cout << "- exception for wrong type: " << e.what() << std::endl;
    }

    // get value without exception
    float* ptr_float = obj.get_if<float>();
    check(ptr_float == nullptr);
    std::string* ptr_string = obj.get_if<std::string>();
    check(ptr_string != nullptr);
    check(*ptr_string == "abc");

    std::cout << std::endl;
}

/*!
 * \brief example of checking types of stored values
 */
void check_type() {
    std::cout << "# Test of checking types of stored values\n" << std::endl;

    variant_cpp11::variant<int, float> obj(1.0F);

    // check whether variant has some value
    check(obj.has_value());
    if (!obj) {
        check(false);
    }

    // check with indices
    std::cout << "- index of value type: " << obj.index() << std::endl;
    check(obj.index() == 1);

    // check with explicit type
    check(!obj.has<int>());
    check(obj.has<float>());

    std::cout << std::endl;
}

/*!
 * \brief example of copying and moving variant objects
 */
void copy_move() {
    std::cout << "# Test of copying and moving variant objects\n" << std::endl;

    // type for test
    struct test_type {
        test_type() = default;
        ~test_type() = default;
        // copying is disabled
        test_type(const test_type&) = delete;
        test_type& operator=(const test_type&) = delete;
        // moving is enabled
        test_type(test_type&&) = default;
        test_type& operator=(test_type&&) = default;
    };

    variant_cpp11::variant<int, test_type> obj(5);

    // copy
    variant_cpp11::variant<int, test_type> copied(obj);

    // move
    variant_cpp11::variant<int, test_type> moved(std::move(copied));

    // copy for types not copyable will throw an exception
    obj = test_type();
    try {
        variant_cpp11::variant<int, test_type> copied = obj;
        std::cout << "- index wrong copied: " << obj.index() << std::endl;
        check(false);
    } catch (variant_cpp11::variant_error& e) {
        std::cout << "- exception for types not copyable: " << e.what()
                  << std::endl;
    }

    std::cout << std::endl;
}

/*!
 * \brief example of executing functions
 */
void execute_functions() {
    std::cout << "# Test of execting functions\n" << std::endl;

    variant_cpp11::variant<int, std::string> obj;

    struct visitor {
        void operator()(int val) {
            std::cout << "- int value: " << val << std::endl;
        }
        void operator()(std::string& val) {
            std::cout << "- string value: " << val << std::endl;
            // visitor can assign values
            val = "abcde";
        }
    };

    obj = 1;
    obj.visit(visitor());
    obj = "a";
    obj.visit(visitor());
    obj.visit(visitor());

    std::cout << std::endl;
}

/*!
 * \brief example of using hash
 */
void use_hash() {
    std::cout << "# Test of using hash\n" << std::endl;

    using key_type = variant_cpp11::variant<int, std::string>;

    // use unordered_map which uses std::hash of variant
    std::unordered_map<key_type, int> hash_map;
    hash_map.emplace(1, 1);
    hash_map.emplace("abc", 2);
    check(hash_map.at(1) == 1);
    check(hash_map.at("abc") == 2);
}

}  // namespace variant_cpp11_examples

int main() {
    variant_cpp11_examples::assign();
    variant_cpp11_examples::get();
    variant_cpp11_examples::check_type();
    variant_cpp11_examples::copy_move();
    variant_cpp11_examples::execute_functions();
    variant_cpp11_examples::use_hash();
    return 0;
}
