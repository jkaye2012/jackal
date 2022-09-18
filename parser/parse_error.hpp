#pragma once

#include <string>
#include <string_view>

#include "lexer/token.hpp"

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

[[nodiscard]] std::string to_string(ErrorType errorType) noexcept;

/// @brief Provides user-centric diagnostic errors to help program authors understand syntax errors
/// in their code.
struct ParseError
{
  /// @brief Constructs a ParseError for an invalid top-level instruction.
  static ParseError invalid_instruction(lexer::Token token, std::string_view message) noexcept;
  /// @brief Constructs a ParseError for an unexpected token.
  static ParseError unexpected_token(lexer::Token token, std::string_view message) noexcept;

  [[nodiscard]] std::string_view message() const noexcept { return _message; }

  void print() const noexcept;

 private:
  ErrorType _type;
  lexer::Token _token;
  std::string _message;

  ParseError(ErrorType type, lexer::Token token, std::string_view message) noexcept;
};
}  // namespace jackal::parser
