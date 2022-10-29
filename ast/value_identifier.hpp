#pragma once

#include <string_view>

#include "lexer/token.hpp"

namespace jackal::ast
{
struct ValueIdentifier
{
  explicit ValueIdentifier(lexer::Token token) noexcept : _token(token) {}

  [[nodiscard]] std::string_view name() const noexcept { return _token.lexeme(); }

  auto operator<=>(ValueIdentifier const&) const noexcept = default;

 private:
  lexer::Token _token;
};
}  // namespace jackal::ast
