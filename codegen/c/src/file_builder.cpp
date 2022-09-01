#include "codegen/c/file_builder.hpp"

#include <sstream>
#include <string>

#include "codegen/c/dependency.hpp"

using jackal::codegen::c::FileBuilder;
using jackal::codegen::c::FunctionCall;
using jackal::codegen::c::FunctionDefinition;

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

FunctionCall::FunctionCall(FileBuilder& fileBuilder, std::string_view name) noexcept
    : _fileBuilder(fileBuilder)
{
  _fileBuilder << name << '(';
}

FunctionCall::~FunctionCall() noexcept { _fileBuilder << ");" << std::endl; }
