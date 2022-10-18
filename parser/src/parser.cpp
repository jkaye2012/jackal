#include "parser/parser.hpp"

#include <charconv>
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>

#include "ast/include.hpp"
#include "ast/visitor.hpp"
#include "parser/include.hpp"
#include "parser/keywords.hpp"
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

// auto ParserV1::parse_program() noexcept -> ProgramResult
// {
//   auto result = ProgramResult::ok_default();
//   while (!_lexer.is_halted())
//   {
//     result.consume(program_subsumer, parse_instruction());
//   }

//   return result;
// }

// auto ParserV1::parse_instruction() noexcept -> InstructionResult
// {
//   auto identifier = expect(lexer::Token::Kind::Identifier);
//   if (identifier.is_err())
//   {
//     return InstructionResult::from(identifier.err());
//   }

//   ast::Instruction::Builder instructionBuilder;
//   if (identifier->lexeme() == keyword::kLet)
//   {
//     auto variable = expect(lexer::Token::Kind::Identifier);
//     if (variable.is_err())
//     {
//       return InstructionResult::from(variable.err());
//     }
//     instructionBuilder.binding.set_variable(variable->lexeme());

//     auto equals = expect(lexer::Token::Kind::Equal);
//     if (equals.is_err())
//     {
//       return InstructionResult::from(equals.err());
//     }

//     auto expression = parse_expression();
//     if (expression.is_err())
//     {
//       return InstructionResult::from(expression.err());
//     }
//     instructionBuilder.binding.set_expression(expression.consume_ok());
//   }
//   else if (identifier->lexeme() == keyword::kPrint)
//   {
//     auto expression = parse_expression();
//     if (expression.is_err())
//     {
//       return InstructionResult::from(expression.err());
//     }
//     instructionBuilder.print.set_expression(expression.consume_ok());
//   }
//   else
//   {
//     return InstructionResult::from(
//         ParseError::invalid_instruction(identifier.ok(), "must begin with 'let' or 'print'\n"));
//   }

//   auto newline = expect(lexer::Token::Kind::Newline);
//   if (newline.is_err())
//   {
//     return InstructionResult::from(newline.err());
//   }

//   return InstructionResult::from(instructionBuilder.build());
// }

// auto ParserV1::parse_expression() noexcept -> ExpressionResult
// {
//   auto value = parse_value();
//   if (value.is_err())
//   {
//     return ExpressionResult::from(value.err());
//   }

//   if (!attempt<0>(lexer::Token::Kind::Plus))
//   {
//     // TODO: force type deduction to work, possible overload using function pointers?
//     std::function<ast::Expression(ast::Value)> func = [](auto val)
//     {
//       return ast::Expression(std::move(val));
//     };
//     return value.consume_map(func);
//   }

//   _lexer.next();  // Eat attempted Plus
//   auto expr = parse_expression();
//   if (expr.is_err())
//   {
//     return ExpressionResult::from(expr.err());
//   }

//   ast::Operator::Builder opBuilder;
//   opBuilder.set_type(ast::Operator::Type::Add);
//   opBuilder.set_a(ast::Expression(value.consume_ok()));
//   opBuilder.set_b(expr.consume_ok());
//   return ExpressionResult::from(ast::Expression(opBuilder.build()));
// }

auto ParserV1::parse_executable() noexcept -> ExecutableResult {}

auto ParserV1::parse_expression() noexcept -> ExpressionResult {}

auto ParserV1::parse_function() noexcept -> FunctionResult {}

auto ParserV1::parse_primitive() noexcept -> PrimitiveResult {}

auto ParserV1::parse_variable() noexcept -> VariableResult {}

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

  auto maybeConstant = attempt<0>(lexer::Token::Kind::Number);
  auto maybeVariable = attempt<0>(lexer::Token::Kind::ValueIdentifier);
  // TODO: this should really attempt to parse a primitive, not specifically a number
  if (maybeVariable.has_value())
  {
    _lexer.next();
    builder.set_local(maybeVariable->lexeme());
  }
  else
  {
    // TODO: parse_primitive must consume next token to advance parsing
    auto primitiveResult = parse_primitive();
    if (primitiveResult.is_err())
    {
      return ValueResult::from(primitiveResult.err());
    }
    // ast::Primitive::Builder primitiveBuilder;
    // primitiveBuilder.number(parse_number());
    valueBuilder.primitive(primitiveResult.consume_ok());
  }

  // TODO: customize error message in case of parsing failure?
  // Thinking that additional context is going to be required.
  // This really brings into question error composition in general, and
  // is something that I'm going to want to include both for eventual
  // real-world use and so that I can more easily debug lexing/parsing issues
  // during development.

  // else
  // {
  //   auto unexpected = _lexer.next();
  //   return ValueResult::from(ParseError::unexpected_token(
  //       unexpected, "malformed value; expected variable or primitive"));
  // }

  return ValueResult::from(valueBuilder.build());
}
