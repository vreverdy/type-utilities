// ========================== EXAMPLE ALL POINTERS ========================== //
// Project:         Type Utilities
// Name:            example_all_pointers.cpp
// Description:     Use cases for [remove/copy/clone]_all_pointers
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2018]
// License:         BSD 3-Clause License
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <vector>
#include <iostream>
// Project sources
#include "../include/type_utilities.hpp"
// Third-party libraries
// Miscellaneous
using namespace type_utilities;
// ========================================================================== //



// ============================= IMPLEMENTATION ============================= //
// Enables if the type is a pointer to a tensor element
template <class T> using enable_if_element_pointer_t = std::enable_if_t<
    std::is_pointer_v<T> && !std::is_pointer_v<std::remove_pointer_t<T>>
>;

// Gets an element: tail
template <class T, class = enable_if_element_pointer_t<T>>
constexpr remove_all_pointers_t<T> get_element(T& tensor) {
    return tensor ? *tensor : remove_all_pointers_t<T>();
}

// Gets an element: recursive call
template <class T, class... Indices>
constexpr remove_all_pointers_t<T> get_element(
    T& tensor, std::size_t idx, Indices&&... idxs
) {
    return tensor && tensor[idx]
        ? get_element(tensor[idx], std::forward<Indices>(idxs)...)
        : remove_all_pointers_t<T>();
}

// Sets an element: tail
template <std::size_t N, class T, class = enable_if_element_pointer_t<T>>
void set_element(const remove_all_pointers_t<T>& val, T& tensor) {
    using raw_t = remove_all_pointers_t<T>;
    tensor ? (*tensor = val, 0) : (tensor = new raw_t(val), 0);
}

// Sets an element: recursive call
template <std::size_t N, class T, class... I>
void set_element(
    const remove_all_pointers_t<T>& val, T& tensor, std::size_t idx, I&&... idxs
) {
    if (!tensor) {
        tensor = new std::remove_pointer_t<T>[N];
        for (std::size_t i = 0; i < N; ++i) tensor[i] = nullptr;
    }
    set_element<N>(val, tensor[idx], std::forward<I>(idxs)...);
}

// Deallocates the entire tensor
template <std::size_t N, class T>
void deallocate(T& tensor) {
    if constexpr (!std::is_pointer_v<std::remove_pointer_t<T>>) {
        delete tensor;
        tensor = nullptr;
    } else if constexpr (std::is_pointer_v<std::remove_pointer_t<T>>) {
        if (tensor) {
            for (std::size_t i = 0; i < N; ++i) deallocate<N>(tensor[i]);
            delete[] tensor;
            tensor = nullptr;
        }
    }
}
// ========================================================================== //



// ================================== MAIN ================================== //
// Main function
int main(int, char**)
{
    // Initialization
    constexpr std::size_t dimension = 4;
    using type = unsigned long long int;
    type***** tensor = nullptr;
    std::size_t i = 0;

    // Fill
    for (std::size_t i0 = 0; i0 < dimension; ++i0) {
        for (std::size_t i1 = 0; i1 < dimension; ++i1) {
            for (std::size_t i2 = 0; i2 < dimension; ++i2) {
                for (std::size_t i3 = 0; i3 < dimension; ++i3) {
                    set_element<dimension>(i++, tensor, i0, i1, i2, i3);
                }
            }
        }
    }

    // Display
    for (std::size_t i0 = 0; i0 < dimension; ++i0) {
        for (std::size_t i1 = 0; i1 < dimension; ++i1) {
            for (std::size_t i2 = 0; i2 < dimension; ++i2) {
                for (std::size_t i3 = 0; i3 < dimension; ++i3) {
                    std::cout << "[" << i0 << ", " << i1 << ", ";
                    std::cout << i2 << ", " << i3 << "]: ";
                    std::cout << get_element(tensor, i0, i1, i2, i3) << "\n";
                }
            }
        }
    }
    
    // Finalization
    deallocate<dimension>(tensor);
    return 0;
}
// ========================================================================== //
