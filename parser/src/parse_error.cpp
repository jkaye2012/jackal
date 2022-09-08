#include "parser/parse_error.hpp"

#include <iostream>
#include <ostream>
#include <string>

using jackal::parser::ErrorType;
using jackal::parser::ParseError;

[[nodiscard]] std::string jackal::parser::to_string(ErrorType errorType) noexcept
{
  switch (errorType)
  {
    case ErrorType::UnexpectedToken:
      return "UnexpectedToken";
    case ErrorType::InvalidInstruction:
      return "InvalidInstruction";
  }
}

ParseError::ParseError(ErrorType type, std::string message) noexcept
    : _type(type), _message(std::move(message))
{
}

ParseError::ParseError(ErrorType type, std::string_view message) noexcept
    : _type(type), _message(message)
{
}

auto ParseError::invalid_instruction(std::string message) noexcept -> ParseError
{
  return {ErrorType::InvalidInstruction, std::move(message)};
}

// TODO: this error messages needs to be greatly improved, even for phased development
auto ParseError::unexpected_token(std::string_view token) noexcept -> ParseError
{
  return {ErrorType::UnexpectedToken, token};
}

auto ParseError::print() const noexcept -> void
{
  std::cerr << "Failed to parse source code:" << std::endl;
  std::cerr << to_string(_type) << ": " << _message << std::endl;
}
