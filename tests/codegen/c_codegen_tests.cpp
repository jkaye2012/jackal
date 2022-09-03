#include <catch.hpp>

#include "ast/operator.hpp"
#include "codegen/c/c_visitor.hpp"
#include "parser/include.hpp"
#include "parser/parse.hpp"

using jackal::codegen::c::CVisitor;
using jackal::parser::Parser;

TEST_CASE("Test code generation")
{
  Parser parser("let x = 1 + 2\nlet y = 3\nprint x + y\n");
  auto result = parser.parse_program();
  CHECKED_ELSE(result.is_ok()) { FAIL(result.err().message()); }

  CVisitor cGen;
  result->accept(cGen);
  FAIL(cGen.generate());
}
