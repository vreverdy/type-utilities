// ============================= TYPE UTILITIES ============================= //
// Project:         Type Utilities
// Name:            type_utilities.cpp
// Description:     Example test file
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2018]
// License:         BSD 3-Clause License
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
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
// ========================================================================== //



// ================================== MAIN ================================== //
// Main function
int main(int argc, char* argv[])
{
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
    print<copy_extent_t<int[], float>>();
    print<copy_extent_t<int[], float[2]>>();
    print<copy_extent_t<int, float[2]>>();
    //print<copy_extent_t<int, float[]>>();
    print<copy_extent_t<int[2][3], float>>();
    std::cout << std::endl;
    
    // Finalization
    return argc ? 0 : argv[0][0];
}
// ========================================================================== //
