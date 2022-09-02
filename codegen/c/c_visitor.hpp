#pragma once

#include "ast/visitor.hpp"
#include "codegen/c/file_builder.hpp"

namespace jackal::codegen::c
{
struct CVisitor : public ast::Visitor
{
  void visit(ast::Operator& node) noexcept override;

  void visit(ast::Expression& node) noexcept override;
  void visit(ast::Binding& node) noexcept override;
  void visit(ast::Print& node) noexcept override;

  void visit(ast::Instruction& node) noexcept override;
  void visit(ast::Program& node) noexcept override;

  void visit(ast::Value& node) noexcept override;
  void visit(ast::Constant& node) noexcept override;
  void visit(ast::LocalVariable& node) noexcept override;

 private:
  FileBuilder _fileBuilder;
};
}  // namespace jackal::codegen::c
