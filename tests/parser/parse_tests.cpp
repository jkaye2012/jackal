#include "ast/operator.hpp"
#include "catch.hpp"
#include "parser/include.hpp"
#include "parser/parse.hpp"

using jackal::parser::Parser;

TEST_CASE("Parsing integer constant should return Value")
{
  Parser parser("123");
  auto result = parser.parse_value();

  CHECKED_ELSE(result.is_ok()) { FAIL(result.err().message()); }
  REQUIRE(result->constant_unsafe().int_unsafe() == 123);
}

TEST_CASE("Parsing double constant should return Value")
{
  Parser parser("1.23");
  auto result = parser.parse_value();

  CHECKED_ELSE(result.is_ok()) { FAIL(result.err().message()); }
  REQUIRE(result->constant_unsafe().double_unsafe() == Approx(1.23));
}

TEST_CASE("Parsing variable identifier should return Value")
{
  Parser parser("foo");
  auto result = parser.parse_value();

  CHECKED_ELSE(result.is_ok()) { FAIL(result.err().message()); }
  REQUIRE(result->local_variable_unsafe().name() == "foo");
}

TEST_CASE("Parsing single binding instruction should return Binding", "[parser]")
{
  Parser parser("let x = 2\n");
  auto result = parser.parse_instruction();

  CHECKED_ELSE(result.is_ok()) { FAIL(result.err().message()); }
  auto const& binding = result->binding_unsafe();
  REQUIRE(binding.variable().name() == "x");
  REQUIRE(binding.expression().value_unsafe().constant_unsafe().int_unsafe() == 2);
}

TEST_CASE("Parsing single print instruction should return Print", "[parser]")
{
  Parser parser("print 1 + 2\n");
  auto result = parser.parse_instruction();

  CHECKED_ELSE(result.is_ok()) { FAIL(result.err().message()); }
  auto const& print = result->print_unsafe();
  REQUIRE(print.expression().operator_unsafe().type() == jackal::ast::Operator::Type::Add);
  REQUIRE(print.expression().operator_unsafe().a().constant_unsafe().int_unsafe() == 1);
  REQUIRE(print.expression().operator_unsafe().b().constant_unsafe().int_unsafe() == 2);
}

TEST_CASE("Parsing program should return all instructions")
{
  Parser parser("let x = 1 + 2\nlet y = 3\nprint x + y\n");
  auto result = parser.parse_program();

  CHECKED_ELSE(result.is_ok()) { FAIL(result.err().message()); }
  REQUIRE(result->instructions().size() == 3);
  REQUIRE(result->instructions().at(0).binding_unsafe().variable().name() == "x");
  REQUIRE(result->instructions().at(1).binding_unsafe().variable().name() == "y");
  REQUIRE(result->instructions()
              .at(2)
              .print_unsafe()
              .expression()
              .operator_unsafe()
              .a()
              .local_variable_unsafe()
              .name() == "x");
}
