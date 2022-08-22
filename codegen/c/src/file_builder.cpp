#include "codegen/c/file_builder.hpp"

#include <sstream>
#include <string>

#include "codegen/c/dependency.hpp"

using jackal::codegen::c::FileBuilder;

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

auto FileBuilder::add_instruction(std::string instruction) noexcept -> void
{
  _instructions.emplace_back(std::move(instruction));
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
  for (auto const& inst : _instructions)
  {
    oss << inst << std::endl;
  }
  oss << '}' << std::endl;

  return oss.str();
}
