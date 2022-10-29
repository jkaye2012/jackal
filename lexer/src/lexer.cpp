#include "lexer/lexer.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string_view>
#include <utility>

#include "lexer/token.hpp"
#include "util/keywords.hpp"

using jackal::lexer::Lexer;

namespace
{
auto is_boolean(std::string_view view) -> bool { return view == "true" || view == "false"; }

auto is_keyword(std::string_view view) -> bool
{
  return std::any_of(jackal::util::keyword::AllKeywords.begin(),
                     jackal::util::keyword::AllKeywords.end(),
                     [view](auto kw)
                     {
                       return kw == view;
                     });
}

auto is_alphalower_or_number(char c) -> bool
{
  return isdigit(c) != 0 || (isalpha(c) != 0 && islower(c) != 0);
}
}  // namespace

auto Lexer::peek() const noexcept -> char { return *_code; }

auto Lexer::peek_n(std::size_t n) const noexcept -> char { return *(_code + n); }

auto Lexer::get() noexcept -> char const*
{
  _gps.column_moved(1);
  return _code++;
}

auto Lexer::get(std::size_t n) noexcept -> std::pair<char const*, char const*>
{
  char const* begin = get();
  for (auto i = n - 1; i > 0; --i)
  {
    get();
  }

  return std::make_pair(begin, _code);
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

auto Lexer::tok_unknown() noexcept -> Token { return tok_unary(Token::Kind::Unknown); }

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

  return {Token::Kind::Number, loc, lexemeBegin, _code};
}

auto Lexer::tok_char() noexcept -> Token
{
  if (peek_n(2) != '\'')
  {
    return tok_unknown();
  }

  auto loc = _gps.current_location();
  auto [begin, end] = get(3);
  return {Token::Kind::Char, loc, begin, end};
}

auto Lexer::tok_string() noexcept -> Token
{
  auto loc = _gps.current_location();
  char const* lexemeBegin = get();
  while (!is_halted() && peek() != '"')
  {
    get();
  }
  if (is_halted())
  {
    return {Token::Kind::Unknown, loc, lexemeBegin, _code};
  }

  get();
  return {Token::Kind::String, loc, lexemeBegin, _code};
}

auto Lexer::tok_alphalower() noexcept -> Token
{
  auto loc = _gps.current_location();
  char const* lexemeBegin = get();
  while (is_alphalower_or_number(peek()))
  {
    get();
  }

  if (peek() != '_')
  {
    std::string_view view(lexemeBegin, _code);
    if (is_boolean(view))
    {
      return {Token::Kind::Boolean, loc, lexemeBegin, _code};
    }
    if (is_keyword(view))
    {
      return {Token::Kind::Keyword, loc, lexemeBegin, _code};
    }
  }

  while (peek() == '_' || is_alphalower_or_number(peek()))
  {
    get();
  }

  return {Token::Kind::ValueIdentifier, loc, lexemeBegin, _code};
}

auto Lexer::tok_type_identifier() noexcept -> Token
{
  auto loc = _gps.current_location();
  char const* lexemeBegin = get();
  while (isalpha(peek()) != 0 || isdigit(peek()) != 0)
  {
    get();
  }

  return {Token::Kind::TypeIdentifier, loc, lexemeBegin, _code};
}

auto Lexer::tok_is() noexcept -> Token
{
  if (peek_n(1) != ':')
  {
    return tok_unknown();
  }

  auto loc = _gps.current_location();
  auto [begin, end] = get(2);
  return {Token::Kind::Is, loc, begin, end};
}

auto Lexer::tok_returns() noexcept -> Token
{
  if (peek_n(1) != '>')
  {
    return tok_unknown();
  }

  auto loc = _gps.current_location();
  auto [begin, end] = get(2);
  return {Token::Kind::Returns, loc, begin, end};
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
  if (current == ';')
  {
    return tok_unary(Token::Kind::End);
  }
  if (current == '=')
  {
    return tok_unary(Token::Kind::Equal);
  }
  if (current == '.')
  {
    return tok_unary(Token::Kind::Dot);
  }
  if (current == ',')
  {
    return tok_unary(Token::Kind::Comma);
  }
  if (current == '{')
  {
    return tok_unary(Token::Kind::OpenScope);
  }
  if (current == '}')
  {
    return tok_unary(Token::Kind::CloseScope);
  }
  if (current == '[')
  {
    return tok_unary(Token::Kind::OpenContext);
  }
  if (current == ']')
  {
    return tok_unary(Token::Kind::CloseContext);
  }
  if (current == '(')
  {
    return tok_unary(Token::Kind::OpenGroup);
  }
  if (current == ')')
  {
    return tok_unary(Token::Kind::CloseGroup);
  }
  if (current == ':')
  {
    return tok_is();
  }
  if (current == '-')
  {
    return tok_returns();
  }
  if (current == '\'')
  {
    return tok_char();
  }
  if (current == '"')
  {
    return tok_string();
  }
  if (isdigit(current) != 0)
  {
    return tok_number();
  }
  if (isalpha(current) != 0)
  {
    if (islower(current) != 0)
    {
      return tok_alphalower();
    }

    return tok_type_identifier();
  }

  return tok_unknown();
}

auto Lexer::is_halted() noexcept -> bool { return _peek.empty() && peek() == '\0'; }
