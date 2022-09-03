#include "codegen/c/file_builder.hpp"

#include <sstream>
#include <string>

#include "codegen/c/dependency.hpp"

using jackal::codegen::c::DirectExpression;
using jackal::codegen::c::FileBuilder;
using jackal::codegen::c::FunctionCall;
using jackal::codegen::c::FunctionDefinition;
using jackal::codegen::c::VariableBinding;

auto FileBuilder::add_dependency(Dependency dep) noexcept -> std::optional<DivergentDependencyError>
{
  if (auto it = _dependencies.find(dep.include()); it != _dependencies.end())
  {
    return it->second != dep ? std::optional(DivergentDependencyError(dep.include()))
                             : std::nullopt;
  }

  _dependencies.emplace(dep.include(), std::move(dep));
  return std::nullopt;
}

auto FileBuilder::build() noexcept -> std::string
{
  std::ostringstream oss;
  for (auto const& [include, dep] : _dependencies)
  {
    switch (dep.type())
    {
      case Dependency::Type::System:
        oss << '<' << include << '>' << std::endl;
        break;
      case Dependency::Type::Source:
        oss << '"' << include << '"' << std::endl;
        break;
    }
  }
  oss << std::endl;

  oss << "int main(int argc, char** argv) {" << std::endl;
  oss << _file.str() << std::endl;
  oss << '}' << std::endl;

  return oss.str();
}

auto FileBuilder::operator<<(std::string_view code) noexcept -> std::ostream&
{
  return _file << code;
}

auto FileBuilder::operator<<(char code) noexcept -> std::ostream& { return _file << code; }

FunctionDefinition::FunctionDefinition(FileBuilder& fileBuilder, std::string_view returnType,
                                       std::string_view name,
                                       std::vector<Parameter> const& params) noexcept
    : _fileBuilder(fileBuilder)
{
  _fileBuilder << returnType << ' ' << name << '(';
  if (!params.empty())
  {
    // TODO: can this be improved using ranges?
    for (int i = 0; i < params.size() - 1; ++i)
    {
      _fileBuilder << params[i].type << ' ' << params[i].name << ", ";
    }
    _fileBuilder << params.back().type << ' ' << params.back().name;
  }
  _fileBuilder << ") {" << std::endl;
}

FunctionCall::FunctionCall(FileBuilder& fileBuilder, std::string_view name) noexcept
    : _fileBuilder(fileBuilder)
{
  _fileBuilder << name << '(';
}

FunctionCall::~FunctionCall() noexcept { _fileBuilder << ");" << std::endl; }

VariableBinding::VariableBinding(FileBuilder& fileBuilder, std::string_view type,
                                 std::string_view name) noexcept
    : _fileBuilder(fileBuilder)
{
  _fileBuilder << type << ' ' << name << " = ";
}

VariableBinding::~VariableBinding() noexcept { _fileBuilder << ';' << std::endl; }

DirectExpression::DirectExpression(FileBuilder& fileBuilder, std::string_view expression) noexcept
    : _fileBuilder(fileBuilder)
{
  _fileBuilder << expression;
}

DirectExpression::~DirectExpression() noexcept = default;
