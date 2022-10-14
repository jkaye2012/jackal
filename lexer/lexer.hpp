#pragma once

#include <cstddef>
#include <iterator>
#include <list>
#include <string_view>
#include <type_traits>
#include <utility>

#include "lexer/gps.hpp"
#include "lexer/token.hpp"

namespace jackal::lexer
{
struct Lexer
{
  static constexpr std::size_t MAX_LOOKAHEAD = 4;

  explicit Lexer(char const* code) : _code(code), _gps(_code) {}

  Token next() noexcept;

  template <std::size_t N,
            std::enable_if_t<N<MAX_LOOKAHEAD, void*> = nullptr> Token peek_token() noexcept
  {
    if (_peek.size() > N)
    {
      auto it = _peek.begin();
      std::advance(it, N);
      return *it;
    }

    if (is_halted())
    {
      return next();
    }

    while (_peek.size() < N + 1 && _peek.back().kind() != Token::Kind::Halt)
    {
      _peek.emplace_back(_next());
    }

    return _peek.back();
  }

  [[nodiscard]] bool is_halted() noexcept;

 private:
  [[nodiscard]] char peek() const noexcept;
  [[nodiscard]] char peek_n(std::size_t n) const noexcept;

  char const* get() noexcept;
  std::pair<char const*, char const*> get(std::size_t n) noexcept;

  Token tok_unary(Token::Kind kind) noexcept;
  Token tok_unknown() noexcept;
  Token tok_number() noexcept;
  Token tok_char() noexcept;
  Token tok_string() noexcept;
  Token tok_alphalower() noexcept;
  Token tok_type_identifier() noexcept;
  Token tok_is() noexcept;
  Token tok_returns() noexcept;

  Token _next() noexcept;

 private:
  char const* _code;
  GPS _gps;
  std::list<Token> _peek;
};
}  // namespace jackal::lexer
