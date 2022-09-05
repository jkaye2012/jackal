#include "codegen/executable.hpp"

#include <fstream>
#include <iostream>

#include "util/exec.hpp"

using jackal::codegen::Executable;

Executable::Executable(std::string name, std::string source) noexcept
    : _name(std::move(name)), _source(std::move(source))
{
}

auto Executable::compile() noexcept -> std::optional<std::string_view>
{
  if (_path.has_value())
  {
    return _path;
  }

  // TODO: fix these paths based on compilation environment
  auto srcPath = "/tmp/" + _name + ".c";
  auto execPath = "/tmp/" + _name + ".out";

  std::ofstream output;
  output.open(srcPath);
  output << _source;
  output.close();

  // TODO: handle linking when required, fix hard-coded C compiler
  auto result = util::exec("/usr/local/bin/clang " + srcPath + " -o " + execPath);
  if (!result.has_value())
  {
    return std::nullopt;
  }

  _path.emplace(std::move(execPath));
  return _path;
}

auto Executable::execute() noexcept -> std::optional<std::string>
{
  auto path = compile();
  if (!path.has_value())
  {
    return std::nullopt;
  }

  return util::exec(*path);
}
