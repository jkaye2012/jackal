#include "parser/parser.hpp"

#include <charconv>
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>

#include "ast/include.hpp"
#include "ast/visitor.hpp"
#include "parser/include.hpp"
#include "util/keywords.hpp"
#include "util/source_location.hpp"

using jackal::parser::ParserV1;

static std::function<void(jackal::ast::Program&, jackal::ast::Instruction)> const program_subsumer =
    [](jackal::ast::Program& program, jackal::ast::Instruction instruction)
{
  program.add_instruction(std::move(instruction));
};

auto ParserV1::expect(lexer::Token::Kind kind) noexcept -> TokenResult
{
  auto token = _lexer.next();
  if (token.kind() == kind)
  {
    return TokenResult::from(token);
  }

  return TokenResult::from(ParseError::unexpected_token(token, "invalid syntax"));
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

auto ParserV1::parse_data() noexcept -> DataResult {}

auto ParserV1::parse_member() noexcept -> MemberResult {}

auto ParserV1::parse_executable() noexcept -> ExecutableResult {}

auto ParserV1::parse_expression() noexcept -> ExpressionResult {}

auto ParserV1::parse_form() noexcept -> FormResult
{
  ast::Form::Builder formBuilder;
  TRY(keywordResult, expect(lexer::Token::Kind::Keyword), FormResult);
  if (keywordResult->lexeme() == util::keyword::Data)
  {
    TRY(dataResult, parse_data(), FormResult);
    formBuilder.data(dataResult.consume_ok());
  }
  if (keywordResult->lexeme() == util::keyword::Fn)
  {
    TRY(functionResult, parse_function(), FormResult);
    formBuilder.function(functionResult.consume_ok());
  }
  else
  {
    return FormResult::from(
        ParseError::unexpected_token(keywordResult.consume_ok(), "unknown top-level form"));
  }

  return FormResult::from(formBuilder.build());
}

auto ParserV1::parse_function() noexcept -> FunctionResult {}

auto ParserV1::parse_function_call() noexcept -> FunctionCallResult {}

auto ParserV1::parse_primitive() noexcept -> PrimitiveResult
{
  // TODO: implement parsing for primitivies other than Number
  ast::Primitive::Builder primitiveBuilder;
  primitiveBuilder.number(parse_number());

  return PrimitiveResult::from(primitiveBuilder.build());
}

auto ParserV1::parse_property_access() noexcept -> PropertyAccessResult {}

auto ParserV1::parse_variable() noexcept -> VariableResult
{
  ast::Variable::Builder variableBuilder;

  TRY(nameResult, expect(lexer::Token::Kind::ValueIdentifier), VariableResult);
  variableBuilder.name(ast::ValueIdentifier(nameResult->lexeme()));

  return VariableResult::from(variableBuilder.build());
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

auto ParserV1::parse_value() noexcept -> ValueResult
{
  ast::ValueV1::Builder valueBuilder;

  if (attempt<0>(lexer::Token::Kind::ValueIdentifier))
  {
    TRY(variableResult, parse_variable(), ValueResult);
    valueBuilder.variable(variableResult.consume_ok());
  }
  else if (peek_primitive())
  {
    // TODO: parse_primitive must consume next token to advance parsing
    TRY(primitiveResult, parse_primitive(), ValueResult);
    valueBuilder.primitive(primitiveResult.consume_ok());
  }
  else
  {
    auto unexpected = _lexer.next();
    return ValueResult::from(ParseError::unexpected_token(
        unexpected, "malformed value; expected variable or primitive"));
  }

  // TODO: customize error message in case of parsing failure?
  // Thinking that additional context is going to be required.
  // This really brings into question error composition in general, and
  // is something that I'm going to want to include both for eventual
  // real-world use and so that I can more easily debug lexing/parsing issues
  // during development.

  return ValueResult::from(valueBuilder.build());
}
