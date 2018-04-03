// ============================= TYPE UTILITIES ============================= //
// Project:         Type Utilities
// Name:            type_utilities.hpp
// Description:     New experimental type traits and type utilities for C++
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2018]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _TYPE_UTILITIES_HPP_INCLUDED
#define _TYPE_UTILITIES_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <iostream>
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace type_utilities {
// ========================================================================== //



/* ****************************** INHERITANCE ******************************* */
// Empty class: blank, empty_class, monostate, empty_base...
struct blank {
};

// Inherit if

/* ************************************************************************** */



/* *************************** TYPE MODIFICATIONS *************************** */
// Copy
template <class From, class To>
struct copy_cv {
    using type = 
};
/* ************************************************************************** */



/* ********************** MISCELLANEOUS TRANSFORMATIONS ********************* */
// Type variadic alias template
template <class T, class...> using type_t = T;

// Value variable template
template <class...>
inline constexpr bool false_v = false;
template <class...>
inline constexpr bool true_v = true;
template <auto V, class...>
inline constexpr auto value_v = V;
/* ************************************************************************** */



/* ***************************** HELPER CLASSES ***************************** */
// Helper alias template for integral constant of type std::size_t
template <std::size_t I>
using index_constant = std::integral_constant<std::size_t, I>;
/* ************************************************************************** */



// ========================================================================== //
} // namespace overload
#endif // _TYPE_UTILITIES_HPP_INCLUDED
// ========================================================================== //
