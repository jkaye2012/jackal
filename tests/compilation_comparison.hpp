#pragma once

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

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
  CompilationComparison(std::string_view inputFileName, std::string_view expectedFileName)
  {
    std::ifstream inputStream(inputFileName.data());
    std::stringstream iss;
    iss << inputStream.rdbuf();
    _input = iss.str();

    std::ifstream expectedStream(inputFileName.data());
    std::stringstream ess;
    ess << expectedStream.rdbuf();
    _expectedOutput = ess.str();
  }

  template <CompilationBackend>
  std::optional<std::string> compile_and_compare();

  template <>
  std::optional<std::string> compile_and_compare<CompilationBackend::C>()
  {
  }

 private:
  std::string _input;
  std::string _expectedOutput;
};
}  // namespace jackal::tests
