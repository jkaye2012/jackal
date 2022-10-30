#include "parser/parser.hpp"

#include <charconv>
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>

#include "ast/include.hpp"
#include "ast/value_identifier.hpp"
#include "ast/visitor.hpp"
#include "lexer/token.hpp"
#include "parser/include.hpp"
#include "util/keywords.hpp"
#include "util/result.hpp"
#include "util/source_location.hpp"

using jackal::parser::ParserV1;

// TODO: should this be moved into the public API for util::Result?

// TODO: newlines are essentially completely unhandled right now, do I want any form of whitespace
// sensitivity in the language?

/// @brief Attempts @p expr, discarding the result if successful or returning an error.
#define TRY_DISCARD(expr)           \
  {                                 \
    auto __discard_trash = (expr);  \
    if (__discard_trash.is_err())   \
    {                               \
      return __discard_trash.err(); \
    }                               \
  }

static std::function<void(jackal::ast::Program&, jackal::ast::Instruction)> const program_subsumer =
    [](jackal::ast::Program& program, jackal::ast::Instruction instruction)
{
  program.add_instruction(std::move(instruction));
};

auto ParserV1::expect(lexer::Token::Kind kind) noexcept -> ParseResult<jackal::lexer::Token>
{
  auto token = _lexer.next();
  if (token.kind() == kind)
  {
    return ParseResult<jackal::lexer::Token>::from(token);
  }

  return ParseResult<jackal::lexer::Token>::from(
      ParseError::unexpected_token(token, "invalid syntax"));
}

template <std::size_t N>
auto ParserV1::attempt(lexer::Token::Kind kind) noexcept -> std::optional<lexer::Token>
{
  auto token = _lexer.peek_token<N>();
  return token.kind() == kind ? std::optional(token) : std::nullopt;
}

bool ParserV1::peek_primitive() noexcept
{
  // TODO: implement other primitive peeks
  return attempt<0>(lexer::Token::Kind::Number).has_value();
}

auto ParserV1::parse_data() noexcept -> ParseResult<ast::Data> {}

auto ParserV1::parse_member() noexcept -> ParseResult<ast::Member> {}

auto ParserV1::parse_executable() noexcept -> ParseResult<ast::Executable> {}

auto ParserV1::parse_expression() noexcept -> ParseResult<ast::Expression> {}

auto ParserV1::parse_form() noexcept -> ParseResult<ast::Form>
{
  ast::Form::Builder formBuilder;
  TRY_ASSIGN(keywordResult, expect(lexer::Token::Kind::Keyword));
  if (keywordResult->lexeme() == util::keyword::Data)
  {
    TRY_ASSIGN(dataResult, parse_data());
    formBuilder.data(dataResult.consume_ok());
  }
  if (keywordResult->lexeme() == util::keyword::Fn)
  {
    TRY_ASSIGN(functionResult, parse_function());
    formBuilder.function(functionResult.consume_ok());
  }
  else
  {
    return ParseResult<ast::Form>::from(
        ParseError::unexpected_token(keywordResult.consume_ok(), "unknown top-level form"));
  }

  return ParseResult<ast::Form>::from(formBuilder.build());
}

auto ParserV1::parse_function() noexcept -> ParseResult<ast::Function>
{
  TRY_ASSIGN(nameResult, expect(lexer::Token::Kind::ValueIdentifier));
  ast::ValueIdentifier name(nameResult.consume_ok());

  ast::Context ctx;
  if (attempt<0>(lexer::Token::Kind::OpenContext))
  {
    TRY_ASSIGN(contextResult, parse_context());
    ctx = contextResult.consume_ok();
  }

  TRY_CONSUME(arguments, parse_arguments());
  TRY_DISCARD(expect(lexer::Token::Kind::Returns));
  TRY_CONSUME(type, parse_type());
  TRY_CONSUME(scope, parse_scope());

  return ParseResult<ast::Function>::from(
      ast::Function(name, std::move(ctx), std::move(arguments), std::move(type), std::move(scope)));
}

auto ParserV1::parse_context() noexcept -> ParseResult<ast::Context>
{
  ast::Context::Builder builder;
  TRY_DISCARD(expect(lexer::Token::Kind::OpenContext));
  while (attempt<0>(lexer::Token::Kind::TypeIdentifier))
  {
    TRY_CONSUME(type, parse_type());
    builder.append_type(std::move(type));

    if (!attempt<0>(lexer::Token::Kind::CloseContext))
    {
      TRY_DISCARD(expect(lexer::Token::Kind::Comma));
    }
  }
  TRY_DISCARD(expect(lexer::Token::Kind::CloseContext));

  return builder.build();
}

auto ParserV1::parse_arguments() noexcept -> ParseResult<ast::Arguments>
{
  ast::Arguments::Builder builder;
  TRY_DISCARD(expect(lexer::Token::Kind::OpenGroup));
  while (attempt<0>(lexer::Token::Kind::ValueIdentifier))
  {
    TRY_CONSUME(nameToken, expect(lexer::Token::Kind::ValueIdentifier));
    TRY_DISCARD(expect(lexer::Token::Kind::Is));
    TRY_CONSUME(type, parse_type());
    builder.append_arg({ast::ValueIdentifier(nameToken), std::move(type)});

    if (!attempt<0>(lexer::Token::Kind::CloseGroup))
    {
      TRY_DISCARD(expect(lexer::Token::Kind::Comma));
    }
  }
  TRY_DISCARD(expect(lexer::Token::Kind::CloseGroup));

  return builder.build();
}

auto ParserV1::parse_type() noexcept -> ParseResult<ast::Type> {}

auto ParserV1::parse_scope() noexcept -> ParseResult<ast::Scope> {}

auto ParserV1::parse_function_call() noexcept -> ParseResult<ast::FunctionCall> {}

auto ParserV1::parse_primitive() noexcept -> ParseResult<ast::Primitive>
{
  // TODO: implement parsing for primitivies other than Number
  ast::Primitive::Builder primitiveBuilder;
  primitiveBuilder.number(parse_number());

  return ParseResult<ast::Primitive>::from(primitiveBuilder.build());
}

auto ParserV1::parse_property_access() noexcept -> ParseResult<ast::PropertyAccess> {}

auto ParserV1::parse_variable() noexcept -> ParseResult<ast::Variable>
{
  ast::Variable::Builder variableBuilder;

  TRY_ASSIGN(nameResult, expect(lexer::Token::Kind::ValueIdentifier));
  variableBuilder.name(ast::ValueIdentifier(nameResult.consume_ok()));

  return ParseResult<ast::Variable>::from(variableBuilder.build());
}

auto ParserV1::parse_number() noexcept -> ast::Number
{
  ast::Number::Builder builder;

  auto lexeme = _lexer.next().lexeme();
  if (lexeme.find('.') == std::string_view::npos)
  {
    int64_t val = 0;
    auto [ptr, _] = std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), val);
    assert(ptr == lexeme.data() + lexeme.size());
    builder.value(val);
  }
  else
  {
    double val = NAN;
    auto [ptr, _] = std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), val);
    assert(ptr == lexeme.data() + lexeme.size());
    builder.value(val);
  }

  return builder.build();
}

auto ParserV1::parse_value() noexcept -> ParseResult<ast::ValueV1>
{
  ast::ValueV1::Builder valueBuilder;

  if (attempt<0>(lexer::Token::Kind::ValueIdentifier))
  {
    TRY_ASSIGN(variableResult, parse_variable());
    valueBuilder.variable(variableResult.consume_ok());
  }
  else if (peek_primitive())
  {
    TRY_ASSIGN(primitiveResult, parse_primitive());
    valueBuilder.primitive(primitiveResult.consume_ok());
  }
  else
  {
    auto unexpected = _lexer.next();
    return ParseResult<ast::ValueV1>::from(ParseError::unexpected_token(
        unexpected, "malformed value; expected variable or primitive"));
  }

  // TODO: customize error message in case of parsing failure?
  // Thinking that additional context is going to be required.
  // This really brings into question error composition in general, and
  // is something that I'm going to want to include both for eventual
  // real-world use and so that I can more easily debug lexing/parsing issues
  // during development.

  return ParseResult<ast::ValueV1>::from(valueBuilder.build());
}
