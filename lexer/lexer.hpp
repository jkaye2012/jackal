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

  constexpr Token next() noexcept;

 private:
  [[nodiscard]] constexpr char const peek() const noexcept;
  [[nodiscard]] constexpr char const peek_n(std::size_t n) const noexcept;
  constexpr char const* get() noexcept;

  constexpr Token tok_unary(Token::Kind kind) noexcept;
  constexpr Token tok_number() noexcept;
  constexpr Token tok_identifier() noexcept;

 private:
  char const* _code;
};
}  // namespace jackal::lexer
