#pragma once

#include <map>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "codegen/c/dependency.hpp"
#include "util/result.hpp"

namespace jackal::codegen::c
{
struct DivergentDependencyError
{
  explicit DivergentDependencyError(std::string_view include) : _include(std::string(include)) {}

  [[nodiscard]] std::string_view include() const noexcept { return _include; }

  [[nodiscard]] std::string message() const noexcept
  {
    std::ostringstream oss;
    oss << "Cannot add duplicate divergent dependency: " << _include << std::endl;
    oss << "This is caused by the same dependency being included by multiple instructions, but "
           "with differing build instructions."
        << std::endl;
    oss << "This is a compiler bug in code generation. Please report it!";
    return oss.str();
  }

 private:
  std::string _include;
};

struct FileBuilder
{
  [[nodiscard]] std::optional<DivergentDependencyError> add_dependency(Dependency dep) noexcept;
  void add_instruction(std::string instruction) noexcept;

  std::string build() noexcept;

 private:
  std::map<std::string_view, Dependency> _dependencies;
  std::vector<std::string> _instructions;
};
}  // namespace jackal::codegen::c
