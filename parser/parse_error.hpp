#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace jackal::parser
{
/// @brief Enumerates all possible classes of error that can be encountered during parsing.
enum class ErrorType
{
  /// @brief A top-level instruction could not be parsed.
  InvalidInstruction,
  /// @brief An unexpected token kind was encountered during parsing.
  UnexpectedToken
};

// TODO: errors should, in general, be combinable so that error propagation can include contextually
// nested information relating to failures. Possibly this could have first-class support in Result,
// but it may also be possible to implement the functionality without coupling the concepts.

/// @brief Provides user-centric diagnostic errors to help program authors understand syntax errors
/// in their code.
struct ParseError
{
  /// @brief Constructs a ParseError for an invalid top-level instruction.
  static ParseError invalid_instruction(std::string message) noexcept;
  /// @brief Constructs a ParseError for an unexpected token.
  static ParseError unexpected_token(std::string_view token) noexcept;

  std::string_view message() const noexcept { return _message; }

 private:
  ErrorType _type;
  std::string _message;
  // TODO: include source location information, shared from util?

  ParseError(ErrorType type, std::string message) noexcept;
  ParseError(ErrorType type, std::string_view message) noexcept;
};
}  // namespace jackal::parser
