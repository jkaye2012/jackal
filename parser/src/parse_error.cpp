#include "parser/parse_error.hpp"

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "lexer/token.hpp"
#include "util/source_location.hpp"

using jackal::parser::ErrorType;
using jackal::parser::ParseError;

[[nodiscard]] std::string jackal::parser::to_string(ErrorType errorType) noexcept
{
  switch (errorType)
  {
    case ErrorType::UnexpectedToken:
      return "unexpected token";
    case ErrorType::InvalidInstruction:
      return "invalid instruction";
  }
}

ParseError::ParseError(ErrorType type, lexer::Token token, std::string_view message) noexcept
    : _type(type), _token(token)
{
  std::ostringstream oss;
  oss << "Failed to parse source code: " << to_string(_type) << " on line "
      << token.location().line().num() << std::endl
      << std::endl;
  oss << util::to_string(_token.location()) << message << std::endl;
  _message = oss.str();
}

auto ParseError::invalid_instruction(lexer::Token token, std::string_view message) noexcept
    -> ParseError
{
  return {ErrorType::InvalidInstruction, token, message};
}

auto ParseError::unexpected_token(lexer::Token token, std::string_view message) noexcept
    -> ParseError
{
  return {ErrorType::UnexpectedToken, token, message};
}

auto ParseError::print() const noexcept -> void { std::cerr << _message; }
