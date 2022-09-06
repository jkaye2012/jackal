#include "codegen/executable.hpp"

#include <fstream>
#include <iostream>

#include "util/exec.hpp"
#include "util/file_system.hpp"

using jackal::codegen::Executable;

Executable::Executable(std::string name, std::string source) noexcept
    : Executable(std::move(name), std::move(source), util::TemporaryDirectory())
{
}

Executable::Executable(std::string name, std::string source,
                       util::TemporaryDirectory&& directory) noexcept
    : _name(std::move(name)), _source(std::move(source)), _dir(std::move(directory))
{
}

auto Executable::compile() noexcept -> std::optional<std::string_view>
{
  if (_path.has_value())
  {
    return _path;
  }

  auto srcPath = _dir.directory() / (_name + ".c");
  auto execPath = _dir.directory() / (_name + ".out");

  std::ofstream output;
  output.open(srcPath);
  output << _source;
  output.close();

  // TODO: handle linking when required, fix hard-coded C compiler
  auto result = util::exec("/usr/local/bin/clang " + srcPath.string() + " -o " + execPath.string());
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
