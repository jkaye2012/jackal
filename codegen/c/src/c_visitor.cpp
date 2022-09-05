#include "codegen/c/c_visitor.hpp"

#include <array>
#include <string>
#include <variant>

#include "ast/include.hpp"
#include "ast/visitor.hpp"
#include "codegen/c/file_builder.hpp"
#include "codegen/executable.hpp"

using jackal::codegen::c::CVisitor;

CVisitor::CVisitor(std::string name) noexcept : _name(std::move(name)) {}

auto CVisitor::visit(ast::Operator& node) noexcept -> void
{
  node.a().accept(*this);
  switch (node.type())
  {
    case jackal::ast::Operator::Type::Add:
      DirectExpression(_fileBuilder, " + ");
      break;
  }
  node.b().accept(*this);
}

auto CVisitor::visit(ast::Expression& node) noexcept -> void
{
  std::visit(
      [this](auto& variant)
      {
        variant.accept(*this);
      },
      node.expression());
}

auto CVisitor::visit(ast::Binding& node) noexcept -> void
{
  auto binding = VariableBinding(_fileBuilder, "int", node.variable().name());
  node.expression().accept(*this);
}

auto CVisitor::visit(ast::Print& node) noexcept -> void
{
  auto result = _fileBuilder.add_dependency({Dependency::Type::System, "stdio.h"});
  // TODO: handle this result properly, forward through type system
  assert(!result.has_value());
  auto call = FunctionCall(_fileBuilder, "printf");
  DirectExpression(_fileBuilder, "\"%d\\n\", ");  // NOLINT
  node.expression().accept(*this);
}

auto CVisitor::visit(ast::Instruction& node) noexcept -> void  // NOLINT
{
  std::visit(
      [this](auto& variant)
      {
        variant.accept(*this);
      },
      node.instruction);
}

auto CVisitor::visit(ast::Program& node) noexcept -> void
{
  for (auto& instr : node.instructions)
  {
    instr.accept(*this);
  }
}

auto CVisitor::visit(ast::Value& node) noexcept -> void
{
  std::visit(
      [this](auto& variant)
      {
        variant.accept(*this);
      },
      node.value());
}

auto CVisitor::visit(ast::Constant& node) noexcept -> void
{
  auto str = std::visit(
      [](auto cnst)
      {
        return std::to_string(cnst);
      },
      node.constant());

  DirectExpression(_fileBuilder, str);
}

auto CVisitor::visit(ast::LocalVariable& node) noexcept -> void
{
  DirectExpression(_fileBuilder, node.name());
}

auto CVisitor::generate() noexcept -> Executable { return {_name, _fileBuilder.build()}; }
