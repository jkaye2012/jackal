#pragma once

#include <cstddef>
#include <fstream>
#include <ios>
#include <string>

#include "lexer/token.hpp"

namespace jackal::lexer
{
struct Lexer
{
  explicit Lexer(char const* code) : _code(code) {}

  Token next() noexcept;

 private:
  [[nodiscard]] char const peek() const noexcept;
  [[nodiscard]] char const peek_n(std::size_t n) const noexcept;
  char const* get() noexcept;

  Token tok_unary(Token::Kind kind) noexcept;
  Token tok_number() noexcept;
  Token tok_identifier() noexcept;

 private:
  char const* _code;
};
}  // namespace jackal::lexer
