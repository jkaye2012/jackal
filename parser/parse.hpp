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

 private:
  [[nodiscard]] util::Result<lexer::Token, ParseError> expect(lexer::Token::Kind kind) noexcept;
  template <std::size_t N>
  [[nodiscard]] std::optional<lexer::Token> attempt(lexer::Token::Kind kind) noexcept;

 private:
  lexer::Lexer _lexer;
};
}  // namespace jackal::parser
