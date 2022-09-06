#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

#include "codegen/c/c_visitor.hpp"
#include "codegen/executable.hpp"
#include "parser/include.hpp"
#include "parser/parse.hpp"

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
  CompilationComparison(std::string name) : _name(std::move(name))
  {
    std::filesystem::path testResourcesDirectory("./tests/resources");

    std::ifstream inputStream(testResourcesDirectory / (_name + ".jkl"));
    std::stringstream iss;
    iss << inputStream.rdbuf();
    _input = iss.str();

    std::ifstream expectedStream(testResourcesDirectory / (_name + ".c"));
    std::stringstream ess;
    ess << expectedStream.rdbuf();
    _expectedCode = ess.str();
  }

  template <CompilationBackend>
  std::optional<std::string> compile_and_compare(std::string_view expectedOutput);

  template <>
  std::optional<std::string> compile_and_compare<CompilationBackend::C>(
      std::string_view expectedOutput)
  {
    parser::Parser parser(_input.c_str());
    auto result = parser.parse_program();

    codegen::c::CVisitor cGen(_name);
    result->accept(cGen);

    auto executable = cGen.generate();
    if (executable.source() != _expectedCode)
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
  std::string _input;
  std::string _expectedCode;
};
}  // namespace jackal::tests
