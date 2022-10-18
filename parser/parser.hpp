#pragma once

#include <optional>

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "parser/results.hpp"

namespace jackal::parser
{
// TODO: rename once old parser is deleted
struct ParserV1
{
  explicit ParserV1(const char *code) noexcept : _lexer(code) {}

  [[nodiscard]] ExecutableResult parse_executable() noexcept;
  [[nodiscard]] ExpressionResult parse_expression() noexcept;
  [[nodiscard]] FunctionResult parse_function() noexcept;
  [[nodiscard]] ValueResult parse_value() noexcept;
  [[nodiscard]] VariableResult parse_variable() noexcept;
  [[nodiscard]] PrimitiveResult parse_primitive() noexcept;
  [[nodiscard]] ast::Number parse_number() noexcept;
  // TODO: top-level parsing of Library

  // TODO: consider incremental parsing for usage in tooling/lsp/etc
  // One simple solution to consider: when a parsing error is encountered,
  // continue to consume tokens until a newline is reached. After each newline,
  // re-attempt parsing. Until this process succeeds, consider all prior errors
  // to be a part of the same "failure"; then, once one succeeds, include all
  // previous failures as part of the error context, then continue parsing as usual.
  //
  // This is likely to result in many semantic errors later in the program, but should
  // at least allow the compiler to construct significantly more information than aborting
  // upon any syntax error.

 private:
  [[nodiscard]] TokenResult expect(lexer::Token::Kind kind) noexcept;

  template <std::size_t N>
  [[nodiscard]] std::optional<lexer::Token> attempt(lexer::Token::Kind kind) noexcept;

 private:
  lexer::Lexer _lexer;
};
}  // namespace jackal::parser
