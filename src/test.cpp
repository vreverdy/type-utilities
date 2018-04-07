// ================================== TEST ================================== //
// Project:         Type Utilities
// Name:            test.cpp
// Description:     Test compilation for all type utilities
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2018]
// License:         BSD 3-Clause License
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <variant>
#include <iostream>
// Project sources
#include "../include/type_utilities.hpp"
// Third-party libraries
// Miscellaneous
using namespace type_utilities;
// ========================================================================== //



// ============================= IMPLEMENTATION ============================= //
// Print utility
template <class T>
void print()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// Inherit if inheritable
template <class T>
struct inheriting
: inherit_if_t<is_inheritable_v<T>, T>
{
};

// Functor
template <class... Args>
struct functor
{
    constexpr void operator()(Args...) noexcept {}
};

// Union functor
template <class... Args>
union ufunctor
{
    constexpr void operator()(Args...) noexcept {}
    constexpr void operator()(Args...) const noexcept {}
};
// ========================================================================== //



// ================================== MAIN ================================== //
// Main function
int main(int, char**)
{
    // Initialization
    auto lambda0 = [](auto x){return x;};
    const auto lambda1 = []{};
    
    // Qualifiers removal
    print<remove_all_pointers_t<int* const* volatile**>>();
    print<remove_cvref_t<const int&&>>();
    std::cout << std::endl;

    // Qualifiers manipulation
    print<copy_const_t<const int, int>>();
    print<copy_const_t<int, const int>>();
    print<clone_const_t<const int, int>>();
    print<clone_const_t<int, const int>>();
    print<copy_volatile_t<volatile int, int>>();
    print<copy_volatile_t<int, volatile int>>();
    print<clone_volatile_t<volatile int, int>>();
    print<clone_volatile_t<int, volatile int>>();
    print<copy_cv_t<volatile int, int>>();
    print<copy_cv_t<int, volatile int>>();
    print<copy_cv_t<const volatile int, volatile int>>();
    print<clone_cv_t<volatile int, int>>();
    print<clone_cv_t<int, volatile int>>();
    print<clone_cv_t<const volatile int, volatile int>>();
    print<copy_reference_t<float, float>>();
    print<copy_reference_t<float&, float>>();
    print<copy_reference_t<float&&, float>>();
    print<copy_reference_t<float, float&>>();
    print<copy_reference_t<float&, float&>>();
    print<copy_reference_t<float&&, float&>>();
    print<copy_reference_t<float, float&&>>();
    print<copy_reference_t<float&, float&&>>();
    print<copy_reference_t<float&&, float&&>>();
    print<clone_reference_t<float, float>>();
    print<clone_reference_t<float&, float>>();
    print<clone_reference_t<float&&, float>>();
    print<clone_reference_t<float, float&>>();
    print<clone_reference_t<float&, float&>>();
    print<clone_reference_t<float&&, float&>>();
    print<clone_reference_t<float, float&&>>();
    print<clone_reference_t<float&, float&&>>();
    print<clone_reference_t<float&&, float&&>>();
    print<copy_signedness_t<char, signed char>>();
    print<copy_signedness_t<signed char, char>>();
    print<copy_signedness_t<unsigned char, char>>();
    print<copy_signedness_t<signed char, unsigned char>>();
    print<copy_extent_t<int, float>>();
    print<copy_extent_t<int, float[]>>();
    print<copy_extent_t<int, float[2]>>();
    print<copy_extent_t<int[], float>>();
    print<copy_extent_t<int[], float[2]>>();
    print<copy_extent_t<int[3], float>>();
    print<copy_extent_t<int[3], float[2]>>();
    print<clone_extent_t<int, float>>();
    print<clone_extent_t<int, float[]>>();
    print<clone_extent_t<int, float[2]>>();
    print<clone_extent_t<int[], float>>();
    print<clone_extent_t<int[], float[]>>();
    print<clone_extent_t<int[], float[2]>>();
    print<clone_extent_t<int[3], float>>();
    print<clone_extent_t<int[3], float[2]>>();
    print<clone_extent_t<int[3], float[2]>>();
    print<copy_all_extents_t<int, double>>();
    print<copy_all_extents_t<int[][2][3][4][5], double>>();
    print<copy_all_extents_t<int[][2][3][4][5], double[6]>>();
    print<clone_all_extents_t<int, double>>();
    print<clone_all_extents_t<int[][2][3][4][5], double>>();
    print<clone_all_extents_t<int[][2][3][4][5], double[6]>>();
    print<copy_pointer_t<int, char>>();
    print<copy_pointer_t<int*, char>>();
    print<copy_pointer_t<int* const, char*>>();
    print<clone_pointer_t<int, char>>();
    print<clone_pointer_t<int*, char>>();
    print<clone_pointer_t<int* const, char*>>();
    print<copy_all_pointers_t<int**, char***>>();
    print<copy_all_pointers_t<int* const* volatile**, char>>();
    print<copy_all_pointers_t<int* const* volatile**, char*>>();
    print<clone_all_pointers_t<int**, char***>>();
    print<clone_all_pointers_t<int* const* volatile**, char>>();
    print<clone_all_pointers_t<int* const* volatile**, char*>>();
    std::cout << std::endl;

    // Conditional inheritance
    std::cout << is_inheritable_v<int>;
    std::cout << is_inheritable_v<std::monostate>;
    std::cout << std::is_base_of_v<std::monostate, inheriting<std::monostate>>;
    std::cout << std::is_base_of_v<int, inheriting<int>>;
    std::cout << std::is_base_of_v<int, inheriting<blank>>;
    std::cout << std::endl << std::endl;

    // Callables categorization
    std::cout << is_closure_v<decltype(lambda0)>;
    std::cout << is_closure_v<decltype(lambda1)>;
    std::cout << is_closure_v<functor<int>>;
    std::cout << is_closure_v<ufunctor<double>>;
    std::cout << is_closure_v<int(int)>;
    std::cout << is_closure_v<int(* const)(int)>;
    std::cout << std::endl;
    std::cout << is_functor_v<decltype(lambda0)>;
    std::cout << is_functor_v<decltype(lambda1)>;
    std::cout << is_functor_v<functor<int>>;
    std::cout << is_functor_v<ufunctor<double>>;
    std::cout << is_functor_v<const ufunctor<double>>;
    std::cout << is_functor_v<int(int)>;
    std::cout << is_functor_v<int(* const)(int)>;
    std::cout << std::endl;
    std::cout << is_function_object_v<decltype(lambda0)>;
    std::cout << is_function_object_v<decltype(lambda1)>;
    std::cout << is_function_object_v<functor<int>>;
    std::cout << is_function_object_v<ufunctor<double>>;
    std::cout << is_function_object_v<const ufunctor<double>>;
    std::cout << is_function_object_v<int(int)>;
    std::cout << is_function_object_v<int(* const)(int)>;
    std::cout << std::endl;
    std::cout << is_callable_v<decltype(lambda0)>;
    std::cout << is_callable_v<decltype(lambda1)>;
    std::cout << is_callable_v<functor<int>>;
    std::cout << is_callable_v<ufunctor<double>>;
    std::cout << is_callable_v<const ufunctor<double>>;
    std::cout << is_callable_v<int(int)>;
    std::cout << is_callable_v<int(* const)(int)>;
    std::cout << std::endl << std::endl;

    // Miscellaneous helpers
    print<index_constant<8>>();
    print<type_t<int, float, double>>();
    std::cout << false_v<int, float, double> << std::endl;
    std::cout << true_v<int, float, double> << std::endl;
    
    // Finalization
    return 0;
}
// ========================================================================== //
