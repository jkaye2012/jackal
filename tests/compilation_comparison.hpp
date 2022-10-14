#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "codegen/c/c_visitor.hpp"
#include "codegen/executable.hpp"
#include "parser/include.hpp"
#include "parser/parse.hpp"
#include "tests/resource.hpp"

namespace jackal::tests
{
// TODO: move this into the actual compiler once multiple backends are supported
enum class CompilationBackend
{
  C
};

/// @brief Compiles a Jackal source file and compares the output to an expected result.
///
/// This comparison can be used arbitrarily across compilation targets so long as the
/// target supports a textual representation.
struct CompilationComparison
{
  CompilationComparison(std::string name)
      : _name(std::move(name)), _input(_name + ".jkl"), _expectedCode(_name + ".jkl")
  {
  }

  template <CompilationBackend>
  std::optional<std::string> compile_and_compare(std::string_view expectedOutput);

  template <>
  std::optional<std::string> compile_and_compare<CompilationBackend::C>(
      std::string_view expectedOutput)
  {
    parser::Parser parser(_input.data());
    auto result = parser.parse_program();

    codegen::c::CVisitor cGen(_name);
    result->accept(cGen);

    auto executable = cGen.generate();
    if (executable.source() != _expectedCode.content())
    {
      return executable.source();
    }

    auto output = executable.execute();
    if (output != expectedOutput)
    {
      return output;
    }

    return std::nullopt;
  }

 private:
  std::string _name;
  FileTestResource _input;
  FileTestResource _expectedCode;
};
}  // namespace jackal::tests
