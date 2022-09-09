#include "parser/parse.hpp"

#include <charconv>
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>

#include "ast/include.hpp"
#include "ast/visitor.hpp"
#include "parser/include.hpp"
#include "parser/keywords.hpp"

using jackal::parser::Parser;
using ProgramResult = jackal::util::Result<jackal::ast::Program, jackal::parser::ParseError>;
using InstructionResult =
    jackal::util::Result<jackal::ast::Instruction, jackal::parser::ParseError>;
using ExpressionResult = jackal::util::Result<jackal::ast::Expression, jackal::parser::ParseError>;
using ValueResult = jackal::util::Result<jackal::ast::Value, jackal::parser::ParseError>;
using TokenResult = jackal::util::Result<jackal::lexer::Token, jackal::parser::ParseError>;

static std::function<void(jackal::ast::Program&, jackal::ast::Instruction)> const program_subsumer =
    [](jackal::ast::Program& program, jackal::ast::Instruction instruction)
{
  program.add_instruction(std::move(instruction));
};

auto Parser::expect(lexer::Token::Kind kind) noexcept -> TokenResult
{
  auto token = _lexer.next();
  if (token.kind() == kind)
  {
    return TokenResult::from(token);
  }

  return TokenResult::from(ParseError::unexpected_token(token.lexeme()));
}

template <std::size_t N>
auto Parser::attempt(lexer::Token::Kind kind) noexcept -> std::optional<lexer::Token>
{
  auto token = _lexer.peek_token<N>();
  return token.kind() == kind ? std::optional(token) : std::nullopt;
}

auto Parser::parse_program() noexcept -> ProgramResult
{
  auto result = ProgramResult::ok_default();
  while (!_lexer.is_halted())
  {
    result.consume(program_subsumer, parse_instruction());
  }

  return result;
}

auto Parser::parse_instruction() noexcept -> InstructionResult
{
  auto identifier = expect(lexer::Token::Kind::Identifier);
  if (identifier.is_err())
  {
    return InstructionResult::from(identifier.err());
  }

  ast::Instruction::Builder instructionBuilder;
  if (identifier->lexeme() == keyword::kLet)
  {
    auto variable = expect(lexer::Token::Kind::Identifier);
    if (variable.is_err())
    {
      return InstructionResult::from(variable.err());
    }
    instructionBuilder.binding.set_variable(variable->lexeme());

    auto equals = expect(lexer::Token::Kind::Equal);
    if (equals.is_err())
    {
      return InstructionResult::from(equals.err());
    }

    auto expression = parse_expression();
    if (expression.is_err())
    {
      return InstructionResult::from(expression.err());
    }
    instructionBuilder.binding.set_expression(expression.consume_ok());
  }
  else if (identifier->lexeme() == keyword::kPrint)
  {
    auto expression = parse_expression();
    if (expression.is_err())
    {
      return InstructionResult::from(expression.err());
    }
    instructionBuilder.print.set_expression(expression.consume_ok());
  }
  else
  {
    return InstructionResult::from(ParseError::invalid_instruction(
        "must begin with 'let' or 'print'; found '" + identifier->lexeme_str() + "'"));
  }

  auto newline = expect(lexer::Token::Kind::Newline);
  if (newline.is_err())
  {
    return InstructionResult::from(newline.err());
  }

  return InstructionResult::from(instructionBuilder.build());
}

auto Parser::parse_expression() noexcept -> ExpressionResult
{
  auto value = parse_value();
  if (value.is_err())
  {
    return ExpressionResult::from(value.err());
  }

  if (!attempt<0>(lexer::Token::Kind::Plus))
  {
    // TODO: force type deduction to work, possible overload using function pointers?
    std::function<ast::Expression(ast::Value)> func = [](auto val)
    {
      return ast::Expression(std::move(val));
    };
    return value.consume_map(func);
  }

  _lexer.next();  // Eat attempted Plus
  auto expr = parse_expression();
  if (expr.is_err())
  {
    return ExpressionResult::from(expr.err());
  }

  ast::Operator::Builder opBuilder;
  opBuilder.set_type(ast::Operator::Type::Add);
  opBuilder.set_a(ast::Expression(value.consume_ok()));
  opBuilder.set_b(expr.consume_ok());
  return ExpressionResult::from(ast::Expression(opBuilder.build()));
}

auto Parser::parse_value() noexcept -> ValueResult
{
  ast::Value::Builder builder;

  auto maybeConstant = attempt<0>(lexer::Token::Kind::Number);
  auto maybeVariable = attempt<0>(lexer::Token::Kind::Identifier);
  if (maybeConstant.has_value())
  {
    _lexer.next();
    auto lexeme = maybeConstant->lexeme();
    if (lexeme.find('.') == std::string_view::npos)
    {
      int64_t val = 0;
      auto [ptr, _] = std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), val);
      assert(ptr == lexeme.data() + lexeme.size());
      builder.set_constant(val);
    }
    else
    {
      double val = NAN;
      auto [ptr, _] = std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), val);
      assert(ptr == lexeme.data() + lexeme.size());
      builder.set_constant(val);
    }
  }
  else if (maybeVariable.has_value())
  {
    _lexer.next();
    builder.set_local(maybeVariable->lexeme());
  }
  else
  {
    auto unexpected = _lexer.next();
    return ValueResult::from(ParseError::unexpected_token(unexpected.lexeme()));
  }

  return ValueResult::from(builder.build());
}
