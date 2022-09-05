#include "parser/parse_error.hpp"

#include <string>

using jackal::parser::ErrorType;
using jackal::parser::ParseError;

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
