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
#if defined(__INTEL_COMPILER)
#define _FUNCTION __PRETTY_FUNCTION__
#define _LAMBDA "lambda ["
#elif defined(__clang__)
#define _FUNCTION __PRETTY_FUNCTION__
#define _LAMBDA "(lambda at"
#elif defined(__GNUC__) || defined(__GNUG__)
#define _FUNCTION __PRETTY_FUNCTION__
#define _LAMBDA "::<lambda"
#elif defined(_MSC_VER)
#define _FUNCTION __FUNCSIG__
#define _LAMBDA "::<lambda"
#else
#define _FUNCTION " "
#define _LAMBDA " "
#endif
// ========================================================================== //



/* ************************* IMPLEMENTATION DETAILS ************************* */
// A placeholder for an argument convertible to anything
struct _argument
{
    template <class T>
    constexpr operator T&() const& noexcept;
    template <class T>
    constexpr operator T&&() const&& noexcept;
};

// Computes the minimum arity of a functor
template <class F, class Args = std::tuple<>, std::size_t N = 128, class = void>
struct _min_arity
{
};

// Computes the minimum arity of a functor: recursive specialization
template <class F, template <class...> class T, class... Args, std::size_t N>
struct _min_arity<F, T<Args...>, N, std::enable_if_t<sizeof...(Args) <= N>>
: std::conditional_t<
    std::is_invocable_v<F, Args...>,
    std::integral_constant<std::size_t, sizeof...(Args)>,
    _min_arity<F, std::tuple<Args..., _argument>, N>
>
{
};

// Checks if a class has a function call operator
template <class F, class = void, class = void>
struct _has_function_call_operator
: std::false_type
{
};

// Checks if a class has a function call operator: minimum arity is defined
template <class F>
struct _has_function_call_operator<
    F,
    std::enable_if_t<std::is_class_v<F> || std::is_union_v<F>>,
    std::void_t<decltype(_min_arity<F>::value)>
>
: std::true_type
{
};

// Checks if a string contains a substring
constexpr bool _contains(const char* string, const char* substring)
{
    constexpr char end = '\0';
    const char* lhs = nullptr;
    const char* rhs = nullptr;
    bool found = *string == end && *substring == end;
    for (; !found && *string != end; ++string) {
        lhs = string;
        rhs = substring;
        for (found = true; found && *lhs != end && *rhs != end; ++lhs, ++rhs) {
            found = *lhs == *rhs;
        }
        found = found && *rhs == end;
    }
    return found;
}

// Checks if a type is a lambda
template <class T, class = std::remove_cv_t<T>>
constexpr bool _is_lambda()
{
    constexpr bool _is_inheritable = std::is_class_v<T> && !std::is_final_v<T>;
    constexpr bool _is_functor = _has_function_call_operator<T>::value;
    return _is_inheritable && _is_functor && _contains(_FUNCTION, _LAMBDA);
}
/* ************************************************************************** */



/* *************************** QUALIFIERS REMOVAL *************************** */
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

// Removes cv and reference qualifiers
template <class T >
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

// Alias templates
template <class T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;
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

// Copies the signedness of an integer
template <class From, class To>
struct copy_signedness
{
    using type = std::conditional_t<
        std::is_same_v<From, std::make_signed_t<From>>,
        std::make_signed_t<To>,
        std::conditional_t<
            std::is_same_v<From, std::make_unsigned_t<From>>,
            std::make_unsigned_t<To>,
            To
        >
    >;
};

// Copies the enclosing array extent
template <class From, class To>
struct copy_extent
{
    using type = std::conditional_t<
        std::rank_v<From> != 0,
        std::conditional_t<
            std::extent_v<From> != 0,
            std::conditional_t<
                std::extent_v<From> != 0,
                To,
                std::remove_all_extents_t<To>
            >[(std::extent_v<From> == 0) + std::extent_v<From>],
            std::conditional_t<
                std::rank_v<From> != 0 && std::extent_v<From> == 0,
                To,
                std::remove_all_extents_t<To>
            >[]
        >,
        To
    >;
};

// Clones the enclosing array extent
template <class From, class To>
struct clone_extent
{
    using type = typename copy_extent<From, std::remove_extent_t<To>>::type;
};

// Copies all array extents
template <class From, class To>
struct copy_all_extents
{
    using type = typename copy_extent<
        From,
        typename std::conditional_t<
            std::rank_v<From> != 0,
            copy_all_extents<std::remove_extent_t<From>, To>,
            copy_extent<From, To>
        >::type
    >::type;
};

// Clones all array extents
template <class From, class To>
struct clone_all_extents
{
    using type = typename copy_all_extents<
        From,
        std::remove_all_extents_t<To>
    >::type;
};

// Copies the enclosing pointer and its cv qualification
template <class From, class To>
struct copy_pointer
{
    using type = std::conditional_t<
        std::is_pointer_v<From>,
        typename copy_cv<From, std::add_pointer_t<To>>::type,
        To
    >;
};

// Clones the enclosing pointer and its cv qualification
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
        typename remove_all_pointers<To>::type
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
        typename remove_cvref<To>::type
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
: std::bool_constant<_is_lambda<T>()>
{
    /* Note: this is a partial implementation and may fail in edge use cases */
};

// Functor detection for class types with an overloaded function call operator
template <class T>
struct is_functor
: std::bool_constant<_has_function_call_operator<T>::value>
{
    /* Note: this is a partial implementation and may fail in edge use cases */
};

// Function object type detection according to [function.objects]
template <class T>
struct is_function_object
: std::bool_constant<is_functor<T>::value || (
    std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>
)>
{
};

// Callable type detection according to [func.def]
template <class T>
struct is_callable
: std::bool_constant<
    is_function_object<T>::value
    || std::is_function_v<T>
    || std::is_member_pointer_v<T>
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
} // namespace type_utilities
#endif // _TYPE_UTILITIES_HPP_INCLUDED
// ========================================================================== //
