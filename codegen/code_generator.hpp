#pragma once

#include <string>

// clang-format off
namespace jackal::codegen { struct Executable; }
// clang-format on

namespace jackal::codegen
{
/// @brief Creates executable code in a target language.
///
/// Jackal may support multiple target languages in the future. CodeGenerator is
/// a simple interface that can be added to Visitor implementations to consistently
/// generate executable code across these targets.
///
/// For now, generated code consists only of the final file output by the end-to-end
/// compilation process (e.g. bytecode for an interpreter, executable out file for C, etc).
/// In the future, this is likely to also include the possibly of dynamic libraries, debug
/// symbols, and more.
struct CodeGenerator
{
  virtual ~CodeGenerator() = default;

  /// @brief Compile and persist an Executable to disk.
  ///
  /// Because CodeGenerator implementations are meant for use alongside Visitor implementations,
  /// this function accepts no parameters. The visitor responsible for generating the code is
  /// expected to incrementally construct its own state internally which can then be used during
  /// final code generation.
  virtual Executable generate() noexcept = 0;
};
}  // namespace jackal::codegen
