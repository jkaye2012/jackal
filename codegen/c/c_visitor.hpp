#pragma once

#include <string>
#include <string_view>

#include "ast/visitor.hpp"
#include "codegen/c/file_builder.hpp"
#include "codegen/code_generator.hpp"

namespace jackal::codegen::c
{
struct CVisitor : public ast::Visitor, public CodeGenerator
{
  explicit CVisitor(std::string name) noexcept;

  void visit(ast::Operator& node) noexcept override;

  void visit(ast::Expression& node) noexcept override;
  void visit(ast::Binding& node) noexcept override;
  void visit(ast::Print& node) noexcept override;

  void visit(ast::Instruction& node) noexcept override;
  void visit(ast::Program& node) noexcept override;

  void visit(ast::Value& node) noexcept override;
  void visit(ast::Constant& node) noexcept override;
  void visit(ast::LocalVariable& node) noexcept override;

  Executable generate() noexcept override;

 private:
  std::string _name;
  FileBuilder _fileBuilder;
};
}  // namespace jackal::codegen::c
