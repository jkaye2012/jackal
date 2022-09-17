#include "lexer/lexer.hpp"

#include <cctype>
#include <cstddef>

#include "lexer/token.hpp"

using jackal::lexer::Lexer;

auto Lexer::peek() const noexcept -> char { return *_code; }

auto Lexer::peek_n(std::size_t n) const noexcept -> char { return *(_code + n); }

auto Lexer::get() noexcept -> char const*
{
  _gps.column_moved(1);
  return _code++;
}

auto Lexer::tok_unary(Token::Kind kind) noexcept -> Token
{
  auto loc = _gps.current_location();
  char const* unary = get();
  if (kind == Token::Kind::Newline)
  {
    _gps.line_moved(_code);
    loc = _gps.current_location();
  }
  return {kind, loc, unary, 1};
}

auto Lexer::tok_number() noexcept -> Token
{
  auto loc = _gps.current_location();
  char const* lexemeBegin = get();
  while (isdigit(peek()) != 0)
  {
    get();
  }
  if (peek() == '.')
  {
    if (isdigit(peek_n(2)) == 0)
    {
      get();
      return {Token::Kind::Unknown, loc, lexemeBegin, _code};
    }

    get();
    get();
  }
  while (isdigit(peek()) != 0)
  {
    get();
  }
  char const* lexemeEnd = _code;

  return {Token::Kind::Number, loc, lexemeBegin, lexemeEnd};
}

auto Lexer::tok_identifier() noexcept -> Token
{
  auto loc = _gps.current_location();
  char const* lexemeBegin = get();
  while (isalpha(peek()) != 0)
  {
    get();
  }
  char const* lexemeEnd = _code;

  return {Token::Kind::Identifier, loc, lexemeBegin, lexemeEnd};
}

auto Lexer::next() noexcept -> Token
{
  if (!_peek.empty())
  {
    auto token = _peek.front();
    _peek.pop_front();
    return token;
  }

  return _next();
}

auto Lexer::_next() noexcept -> Token
{
  while (peek() == ' ')
  {
    get();
  }

  char const current = peek();
  if (current == '\0')
  {
    return {Token::Kind::Halt, _gps.current_location(), _code, 1};
  }
  if (current == '\n')
  {
    return tok_unary(Token::Kind::Newline);
  }
  if (current == '+')
  {
    return tok_unary(Token::Kind::Plus);
  }
  if (current == '=')
  {
    return tok_unary(Token::Kind::Equal);
  }
  if (isdigit(current) != 0)
  {
    return tok_number();
  }
  if (isalpha(current) != 0 && islower(current) != 0)
  {
    return tok_identifier();
  }

  return tok_unary(Token::Kind::Unknown);
}

auto Lexer::is_halted() noexcept -> bool { return _peek.empty() && peek() == '\0'; }
