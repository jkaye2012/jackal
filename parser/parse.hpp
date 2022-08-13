#pragma once

#include <optional>

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"

// clang-format off
namespace jackal::ast { struct Expression; }
namespace jackal::ast { struct Instruction; }
namespace jackal::ast { struct Program; }
namespace jackal::ast { struct Value; }
namespace jackal::parser { struct ParseError; }
namespace jackal::util { template<typename, typename> struct Result; }
// clang-format on

namespace jackal::parser
{
struct Parser
{
  explicit Parser(const char *code) noexcept : _lexer(code) {}

  [[nodiscard]] util::Result<ast::Program, ParseError> parse_program() noexcept;
  [[nodiscard]] util::Result<ast::Instruction, ParseError> parse_instruction() noexcept;
  [[nodiscard]] util::Result<ast::Expression, ParseError> parse_expression() noexcept;
  [[nodiscard]] util::Result<ast::Value, ParseError> parse_value() noexcept;

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
  [[nodiscard]] util::Result<lexer::Token, ParseError> expect(lexer::Token::Kind kind) noexcept;
  template <std::size_t N>
  [[nodiscard]] std::optional<lexer::Token> attempt(lexer::Token::Kind kind) noexcept;

 private:
  lexer::Lexer _lexer;
};
}  // namespace jackal::parser
