#include <catch.hpp>

#include "tests/compilation_comparison.hpp"

using jackal::tests::CompilationBackend;
using jackal::tests::CompilationComparison;

TEST_CASE("C code generation: printing constant valued addition", "[codegen_c]")
{
  CompilationComparison comparison("print_values");
  auto result = comparison.compile_and_compare<CompilationBackend::C>("4\n");
  CHECKED_ELSE(!result.has_value()) { FAIL(*result); }
}

TEST_CASE("C code generation: printing a single bound variable", "[codegen_c]")
{
  CompilationComparison comparison("print_variable");
  auto result = comparison.compile_and_compare<CompilationBackend::C>("125\n");
  CHECKED_ELSE(!result.has_value()) { FAIL(*result); }
}

TEST_CASE("C code generation: printing addition of multiple bound variables", "[codegen_c]")
{
  CompilationComparison comparison("print_expression");
  auto result = comparison.compile_and_compare<CompilationBackend::C>("10\n10\n");
  CHECKED_ELSE(!result.has_value()) { FAIL(*result); }
}
