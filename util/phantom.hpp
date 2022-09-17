#pragma once

#include <utility>

namespace jackal::util
{
/// @brief A zero-cost type-safety wrapper.
///
/// Phantom allows logical constructions that would normally be applied at run-time
/// to be lifted into the type system. For example:
///
/// <ul>
/// <li>Conceptual types that share the same underlying storage can be disambiguated</li>
/// <li>Types can be given "states" allowing for a sort of compile-time state machine</li>
/// </ul>
///
/// Usually, the simplest way to work with Phantoms is to use empty structs as the phantom tag.
/// For example, we may wish to define a "Counter" type using an unsigned long as its underlying
/// storage, but we want to ensure that this counter's value cannot accidentally be mistaken for
/// any other uint64_t:
///
/// @code
/// struct CounterTag {};
/// using Counter = Phantom<CounterTag, uint64_t>;
/// Counter counter(uint64_t value) noexcept { return {value}; }
/// @endcode
///
/// @tparam P the phantom tag; does not have any run-time representation
/// @tparam T the underlying type wrapped by the Phantom
template <typename P, typename T>
struct Phantom
{
  /// @brief Constructs a Phantom from an rvalue reference.
  constexpr Phantom(T&& inner) noexcept : value(std::move(inner)) {}

  /// @brief Constructs a Phantom from a constant lvalue reference
  constexpr Phantom(T const& inner) noexcept : value(inner) {}

  T value;
};
}  // namespace jackal::util
