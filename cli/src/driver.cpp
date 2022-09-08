#include "cli/driver.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "cli/options.hpp"
#include "codegen/c/c_visitor.hpp"
#include "codegen/executable.hpp"
#include "parser/include.hpp"
#include "parser/parse.hpp"
#include "util/exit.hpp"
#include "util/file_system.hpp"
#include "util/result.hpp"

using jackal::cli::Driver;

Driver::Driver(Options const& options) noexcept
{
  std::filesystem::path filePath(options.file_path());
  auto file = util::read_file(filePath);
  if (!file.has_value())
  {
    std::cerr << "Could not read source file '" << filePath << "'" << std::endl;
    std::exit(util::ExitMissingSource);
  }

  parser::Parser parser(file->c_str());
  auto parseResult = parser.parse_program();
  if (parseResult.is_err())
  {
    parseResult.err().print();
    std::exit(util::ExitSyntaxError);
  }

  codegen::c::CVisitor codeGenerator(filePath.stem());
  parseResult->accept(codeGenerator);

  auto executable = codeGenerator.generate();
  auto executablePath = executable.compile();
  if (!executablePath.has_value())
  {
    std::cerr << "Failed to compile executable" << std::endl;
    std::exit(util::ExitCodeGenerationFailed);
  }

  // TODO: this can throw an exception, handle properly
  std::filesystem::copy(std::filesystem::path(*executablePath), options.output_directory());
}
