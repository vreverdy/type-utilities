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
    // Pointers removal
    std::cout << std::is_same_v<
        remove_all_pointers_t<int* const* volatile**>,
        int
    > << std::endl;
    std::cout << std::endl;

    // Conditional inheritance
    std::cout << std::is_base_of_v<blank, inheriting<int>> << std::endl;
    std::cout << std::is_base_of_v<
        std::bool_constant<false>,
        inheriting<std::bool_constant<false>>
    > << std::endl;
    std::cout << std::endl;

    // Qualifiers manipulation
    
    
    // Finalization
    return argc ? 0 : argv[0][0];
}
// ========================================================================== //
