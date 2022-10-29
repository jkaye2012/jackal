#pragma once

#include <optional>

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "parser/results.hpp"

// clang-format off
namespace jackal::ast { struct Arguments; }
namespace jackal::ast { struct Context; }
namespace jackal::ast { struct Data; }
namespace jackal::ast { struct Executable; }
namespace jackal::ast { struct Expression; }
namespace jackal::ast { struct Form; }
namespace jackal::ast { struct Function; }
namespace jackal::ast { struct FunctionCall; }
namespace jackal::ast { struct Member; }
namespace jackal::ast { struct Number; }
namespace jackal::ast { struct Primitive; }
namespace jackal::ast { struct PropertyAccess; }
namespace jackal::ast { struct ValueV1; }
namespace jackal::ast { struct Variable; }
namespace jackal::ast { struct Scope; }
namespace jackal::ast { struct Type; }
namespace jackal::lexer { struct Token; }
// clang-format on

namespace jackal::parser
{
// TODO: rename once old parser is deleted
struct ParserV1
{
  explicit ParserV1(const char *code) noexcept : _lexer(code) {}

  [[nodiscard]] ParseResult<jackal::ast::Arguments> parse_arguments() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Context> parse_context() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Data> parse_data() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Member> parse_member() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Executable> parse_executable() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Expression> parse_expression() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Form> parse_form() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Function> parse_function() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::FunctionCall> parse_function_call() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::ValueV1> parse_value() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Variable> parse_variable() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Primitive> parse_primitive() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::PropertyAccess> parse_property_access() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Type> parse_type() noexcept;
  [[nodiscard]] ParseResult<jackal::ast::Scope> parse_scope() noexcept;
  // While it might seem odd that this returns a Number directly instead of a result,
  // this is because the only way for this parsing to fail is a bug in the lexer;
  // therefore, we rely upon assertions within the parser to cause compilation to fail
  // loudly to alert us of this bug.
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
  [[nodiscard]] ParseResult<jackal::lexer::Token> expect(lexer::Token::Kind kind) noexcept;

  template <std::size_t N>
  [[nodiscard]] std::optional<lexer::Token> attempt(lexer::Token::Kind kind) noexcept;

  [[nodiscard]] bool peek_primitive() noexcept;

 private:
  lexer::Lexer _lexer;
};
}  // namespace jackal::parser
