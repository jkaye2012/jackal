#pragma once

#include <cstddef>
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

// TODO: handle indentation
struct FileBuilder
{
  [[nodiscard]] std::optional<DivergentDependencyError> add_dependency(Dependency dep) noexcept;

  std::ostream& operator<<(std::string_view code) noexcept;
  std::ostream& operator<<(char code) noexcept;

  std::string build() noexcept;

 private:
  std::map<std::string_view, Dependency> _dependencies;
  std::ostringstream _file;
};

struct Parameter
{
  Parameter(std::string_view type, std::string_view name) : type(type), name(name) {}

  std::string_view const type;
  std::string_view const name;
};

struct FunctionDefinition
{
  template <std::size_t N>
  FunctionDefinition(FileBuilder& fileBuilder, std::string_view returnType, std::string_view name,
                     std::array<Parameter, N> const& params) noexcept
      : _fileBuilder(fileBuilder)
  {
    _fileBuilder << returnType << ' ' << name << '(';
    if constexpr (N > 0)
    {
      for (int i = 0; i < params.size() - 1; ++i)
      {
        _fileBuilder << params[i].type << ' ' << params[i].name << ", ";
      }
      _fileBuilder << params.back().type << ' ' << params.back().name;
    }
    _fileBuilder << ") {" << std::endl;
  }

  FunctionDefinition(FileBuilder& fileBuilder, std::string_view returnType, std::string_view name,
                     std::vector<Parameter> const& params) noexcept;

  ~FunctionDefinition() noexcept { _fileBuilder << '}' << std::endl; }

  FunctionDefinition(FunctionDefinition const&) = delete;
  FunctionDefinition& operator=(FunctionDefinition const&) = delete;
  FunctionDefinition(FunctionDefinition&&) noexcept = delete;
  FunctionDefinition& operator=(FunctionDefinition&&) noexcept = delete;

 private:
  FileBuilder& _fileBuilder;
};

struct FunctionCall
{
  FunctionCall(FileBuilder& fileBuilder, std::string_view name) noexcept;
  ~FunctionCall() noexcept;

  FunctionCall(FunctionCall const&) = delete;
  FunctionCall& operator=(FunctionCall const&) = delete;
  FunctionCall(FunctionCall&&) noexcept = delete;
  FunctionCall& operator=(FunctionCall&&) noexcept = delete;

 private:
  FileBuilder& _fileBuilder;
};

struct VariableBinding
{
  VariableBinding(FileBuilder& fileBuilder, std::string_view type, std::string_view name) noexcept;
  ~VariableBinding() noexcept;

  VariableBinding(VariableBinding const&) = delete;
  VariableBinding& operator=(VariableBinding const&) = delete;
  VariableBinding(VariableBinding&&) noexcept = delete;
  VariableBinding& operator=(VariableBinding&&) noexcept = delete;

 private:
  FileBuilder& _fileBuilder;
};

struct DirectExpression
{
  DirectExpression(FileBuilder& fileBuilder, std::string_view expression) noexcept;
  ~DirectExpression() noexcept;

  DirectExpression(DirectExpression const&) = delete;
  DirectExpression& operator=(DirectExpression const&) = delete;
  DirectExpression(DirectExpression&&) noexcept = delete;
  DirectExpression& operator=(DirectExpression&&) noexcept = delete;

 private:
  FileBuilder& _fileBuilder;
};
}  // namespace jackal::codegen::c
