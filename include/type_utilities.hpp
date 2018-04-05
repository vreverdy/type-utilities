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
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace type_utilities {
// ========================================================================== //



/* **************************** POINTERS REMOVAL **************************** */
// Removes all pointers from a type
template <class T>
struct remove_all_pointers
{
    using type = typename std::conditional_t<
        std::is_pointer_v<T>,
        remove_all_pointers<std::remove_pointer_t<T>>,
        std::remove_pointer<T>
    >::type;
};

// Alias template
template <class T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;
/* ************************************************************************** */



/* ************************* QUALIFIERS MANIPULATION ************************ */
// Copies the const qualifier
template <class From, class To>
struct copy_const
{
    using type = std::conditional_t<
        std::is_const_v<From>,
        std::add_const_t<To>,
        To
    >;
};

// Clones the const qualifier
template <class From, class To>
struct clone_const
{
    using type = typename copy_const<From, std::remove_const_t<To>>::type;
};

// Copies the volatile qualifier
template <class From, class To>
struct copy_volatile
{
    using type = std::conditional_t<
        std::is_volatile_v<From>,
        std::add_volatile_t<To>,
        To
    >;
};

// Clones the volatile qualifier
template <class From, class To>
struct clone_volatile
{
    using type = typename copy_volatile<From, std::remove_volatile_t<To>>::type;
};

// Copies cv qualifiers
template <class From, class To>
struct copy_cv
{
    using type = typename copy_const<
        From,
        typename copy_volatile<From, To>::type
    >::type;
};

// Clones cv qualifiers
template <class From, class To>
struct clone_cv
{
    using type = typename copy_cv<From, std::remove_cv_t<To>>::type;
};

// Copies the reference qualifier
template <class From, class To>
struct copy_reference
{
    using type = std::conditional_t<
        std::is_rvalue_reference_v<From>,
        std::add_rvalue_reference_t<To>,
        std::conditional_t<
            std::is_lvalue_reference_v<From>,
            std::add_lvalue_reference_t<To>,
            To
        >
    >;
};

// Clones the reference qualifier
template <class From, class To>
struct clone_reference
{
    using type = typename copy_reference<
        From,
        std::remove_reference_t<To>
    >::type;
};

// Copies the most external pointer and its cv qualification
template <class From, class To>
struct copy_pointer
{
    using type = std::conditional_t<
        std::is_pointer_v<From>,
        typename copy_cv<From, std::add_pointer_t<To>>,
        To
    >;
};

// Clones the most external pointer and its cv qualification
template <class From, class To>
struct clone_pointer
{
    using type = typename copy_pointer<From, std::remove_pointer_t<To>>::type;
};

// Copies all pointers and their cv qualification
template <class From, class To>
struct copy_all_pointers
{
    using type = typename copy_pointer<
        From,
        typename std::conditional_t<
            std::is_pointer_v<From>,
            copy_all_pointers<std::remove_pointer_t<From>, To>,
            copy_pointer<From, To>
        >::type
    >::type;
};

// Clones all pointers and their cv qualification
template <class From, class To>
struct clone_all_pointers
{
    using type = typename copy_all_pointers<
        From,
        std::remove_all_pointers_t<To>
    >::type;
};

// Copies cv and reference qualifiers
template <class From, class To>
struct copy_cvref
{
    using type = typename copy_reference<
        From,
        typename copy_reference<
            To,
            typename copy_cv<
                std::remove_reference_t<From>,
                std::remove_reference_t<To>
            >::type
        >::type
    >::type;
};

// Clones cv and reference qualifiers
template <class From, class To>
struct clone_cvref
{
    using type = typename copy_cvref<
        From,
        typename std::remove_cvref<To>::type
    >::type;
};

// Alias templates
template <class From, class To>
using copy_const_t = typename copy_const<From, To>::type;
template <class From, class To>
using clone_const_t = typename clone_const<From, To>::type;
template <class From, class To>
using copy_volatile_t = typename copy_volatile<From, To>::type;
template <class From, class To>
using clone_volatile_t = typename clone_volatile<From, To>::type;
template <class From, class To>
using copy_cv_t = typename copy_cv<From, To>::type;
template <class From, class To>
using clone_cv_t = typename clone_cv<From, To>::type;
template <class From, class To>
using copy_reference_t = typename copy_reference<From, To>::type;
template <class From, class To>
using clone_reference_t = typename clone_reference<From, To>::type;
template <class From, class To>
using copy_signedness_t = typename copy_signedness<From, To>::type;
template <class From, class To>
using copy_extent_t = typename copy_extent<From, To>::type;
template <class From, class To>
using clone_extent_t = typename clone_extent<From, To>::type;
template <class From, class To>
using copy_all_extents_t = typename copy_all_extents<From, To>::type;
template <class From, class To>
using clone_all_extents_t = typename clone_all_extents<From, To>::type;
template <class From, class To>
using copy_pointer_t = typename copy_pointer<From, To>::type;
template <class From, class To>
using clone_pointer_t = typename clone_pointer<From, To>::type;
template <class From, class To>
using copy_all_pointers_t = typename copy_all_pointers<From, To>::type;
template <class From, class To>
using clone_all_pointers_t = typename clone_all_pointers<From, To>::type;
template <class From, class To>
using copy_cvref_t = typename copy_cvref<From, To>::type;
template <class From, class To>
using clone_cvref_t = typename clone_cvref<From, To>::type;
/* ************************************************************************** */



/* ************************* CONDITIONAL INHERITANCE ************************ */
// A universal empty class
struct blank
{
};

// Checks if a type can serve as a base class
template <class T>
struct is_inheritable
: std::bool_constant<std::is_class_v<T> && !std::is_final_v<T>>
{
};

// Inherits from a type if a condition is met
template <bool Condition, class T>
struct inherit_if
{
    using type = std::conditional_t<Condition, T, blank>;
};

// Type alias and variable template
template <class T>
inline constexpr bool is_inheritable_v = is_inheritable<T>::value;
template <bool Condition, class T>
using inherit_if_t = typename inherit_if<Condition, T>::type;
/* ************************************************************************** */



/* ************************ CALLABLES CATEGORIZATION ************************ */
// Closure type detection according to [expr.prim.lambda.closure]
template <class T>
struct is_closure
{
    static_assert("Not implemented yet");
};

// Functor detection for class types with an overloaded function call operator
template <class T>
struct is_functor
{
    static_assert("Not implemented yet");
};

// Function object type detection according to [function.objects]
template <class T>
struct is_function_object
{
    static_assert("Not implemented yet");
};

// Callable type detection according to [func.def]
template <class T>
struct is_callable
: std::bool_constant<
    is_function_object<T>::value || std::is_member_pointer_v<T>
>
{    
};

// Variable templates
template <class T>
inline constexpr bool is_closure_v = is_closure<T>::value;
template <class T>
inline constexpr bool is_functor_v = is_functor<T>::value;
template <class T>
inline constexpr bool is_function_object_v = is_function_object<T>::value;
template <class T>
inline constexpr bool is_callable_v = is_callable<T>::value;
/* ************************************************************************** */



/* ************************** MISCELLANEOUS HELPERS ************************* */
// Index contant alias template
template <std::size_t I>
using index_constant = std::integral_constant<std::size_t, I>;

// Type variadic alias template
template <class T, class...>
using type_t = T;

// False and true variable templates
template <class...>
inline constexpr bool false_v = false;
template <class...>
inline constexpr bool true_v = true;
/* ************************************************************************** */



// ========================================================================== //
} // namespace overload
#endif // _TYPE_UTILITIES_HPP_INCLUDED
// ========================================================================== //
