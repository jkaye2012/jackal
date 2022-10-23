#pragma once

#include <functional>
#include <utility>
#include <variant>

namespace jackal::util
{
/// @brief Attempts to define a local variable with the result of an expression, returning n
/// marshalled error from the current function if the expression fails.
///
/// @param var the name of the local variable to define if the expression is successful
/// @param expr the expression that will be invoked to define @p var. Must return a Result
/// with an Err that matches @p ErrResult
/// @param ErrResult the error type to marshall failures. Must be a Result with an Err that
/// matches the failure case of @p expr
#define TRY(var, expr, ErrResult)        \
  auto(var) = (expr);                    \
  if ((var).is_err())                    \
  {                                      \
    return ErrResult::from((var).err()); \
  }

/// @brief Models the possibility of failure for an operation.
///
/// Jackal's implementation uses Result over exceptions wherever possible.
/// Exceptions are thrown only when they are not meant to be handled (e.g. when a bug or programmer
/// error is encountered).
///
/// Result provides a basic monadic interface to simplify contextual error
/// handling and to facilitate a more "functional" style of composition.
///
/// A successful Result is referred to as "ok", while a failed Result is referred to as an "err".
///
/// Does not support T and Err of the same type.
///
/// @tparam T the type contained by a successful operation
/// @tparam Err the type contained by a failed operation
template <typename T, typename Err>
struct Result
{
  /// @brief Constructs an ok Result.
  ///
  /// @param result the value to wrap
  /// @returns an ok Result
  static constexpr Result from(T result) noexcept { return Result(std::move(result)); }

  /// @brief Constructs an err Result.
  ///
  /// @param error the error to wrap
  /// @returns an err Result
  static constexpr Result from(Err error) noexcept { return Result(std::move(error)); }

  /// @brief Constructs an ok Result using the default constructor of @p T.
  ///
  /// @returns an ok Result
  static constexpr Result ok_default() noexcept { return Result(T()); }

  /// @brief Applies a function to the Result if it is ok, does nothing if it is err.
  ///
  /// @param func the function to apply to an ok Result
  /// @returns the mapped Result
  template <typename U>
  [[nodiscard]] constexpr Result<U, Err> map(std::function<U(T const&)> const& func) const noexcept
  {
    if (is_ok())
    {
      return Result<U, Err>::from(func(ok()));
    }
    return Result<U, Err>::from(err());
  }
  /// @brief Destructively applies a function to the Result if it is ok, does nothing if it is err.
  ///
  /// @param func the function to apply to an ok Result
  /// @returns the mapped Result
  template <typename U>
  [[nodiscard]] constexpr Result<U, Err> consume_map(std::function<U(T)> const& func) noexcept
  {
    if (is_ok())
    {
      return Result<U, Err>::from(func(consume_ok()));
    }
    return Result<U, Err>::from(err());
  }

  /// @brief Applies a function to the Result if it is err, does nothing if it is ok.
  ///
  /// @param func the function to apply to an err Result
  /// @returns the mapped Result
  template <typename UErr>
  [[nodiscard]] constexpr Result<T, UErr> map_err(
      std::function<UErr(Err const&)> const& func) const noexcept
  {
    if (is_ok())
    {
      return Result<T, UErr>::from(ok());
    }
    return Result<T, UErr>::from(func(err()));
  }

  /// @brief Applies a function to the Result depending upon its state.
  ///
  /// @param okFunc the function to apply to an ok Result
  /// @param errFunc the function to apply to an err Result
  /// @returns the mapped Result
  template <typename U, typename UErr>
  [[nodiscard]] constexpr Result<U, UErr> map_flat(
      std::function<U(T const&)> const& okFunc,
      std::function<UErr(Err const&)> const& errFunc) const noexcept
  {
    if (is_ok())
    {
      return Result<U, UErr>::from(okFunc(ok()));
    }
    return Result<U, UErr>::from(errFunc(err()));
  }

  /// @brief Combines another Result into the current instance depending on mutual state.
  ///
  /// If the current instance is err, the operation is a no-op.
  /// Otherwise, if the other instance is err, the value of the current instance is replaced by the
  /// error. If neither are err (both are ok), @p func is used to update the current instance's
  /// value.
  ///
  /// @param func the subsumption function to apply to the current instance if both Results are ok
  /// @param other the Result instance to subsume
  template <typename U>
  constexpr void subsume(std::function<void(T&, U const&)> const& func,
                         Result<U, Err> const& other) noexcept
  {
    if (is_err())
    {
      return;
    }
    if (other.is_err())
    {
      _result.template emplace<Err>(other.err());
    }
    else
    {
      T& current = std::get<T>(_result);
      func(current, other.ok());
    }
  }

  /// @brief Destructively combines another Result into the current instance depending on mutual
  /// state.
  ///
  /// @see subsume for a more in-depth description.
  /// @param func the consumption function to apply to the current instance if both Results are ok
  /// @param other the Result instance to consume
  template <typename U>
  constexpr void consume(std::function<void(T&, U)> const& func, Result<U, Err> other) noexcept
  {
    if (is_err())
    {
      return;
    }
    if (other.is_err())
    {
      _result.template emplace<Err>(other.err());
    }
    else
    {
      T& current = std::get<T>(_result);
      func(current, other.consume_ok());
    }
  }

  constexpr void operator>>(std::function<void()> const& func) noexcept
  {
    if (is_ok())
    {
      func();
    }
  }

  constexpr T const* operator->() const noexcept { return std::get_if<T>(&_result); }
  constexpr T* operator->() noexcept { return std::get_if<T>(&_result); }

  /// @returns whether the Result is in the ok state
  [[nodiscard]] constexpr bool is_ok() const noexcept { return std::holds_alternative<T>(_result); }

  /// @returns the value of an ok Result; terminates the program if the Result is err
  [[nodiscard]] constexpr T ok() const noexcept { return std::get<T>(_result); }

  /// @returns the moved value of an ok Result; terminates the program if the Result is err
  [[nodiscard]] constexpr T consume_ok() noexcept { return std::move(std::get<T>(_result)); }

  /// @returns whether the Result in in the err state
  [[nodiscard]] constexpr bool is_err() const noexcept
  {
    return std::holds_alternative<Err>(_result);
  }

  /// @returns the value of an err Result; terminates the program if the Result is ok
  [[nodiscard]] constexpr Err err() const noexcept { return std::get<Err>(_result); }

 private:
  explicit constexpr Result(T result) noexcept : _result(std::move(result)) {}
  explicit constexpr Result(Err error) noexcept : _result(std::move(error)) {}

  std::variant<T, Err> _result;
};
}  // namespace jackal::util
