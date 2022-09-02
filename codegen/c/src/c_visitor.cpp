#include "codegen/c/c_visitor.hpp"

#include "ast/include.hpp"

using jackal::codegen::c::CVisitor;

auto CVisitor::visit(ast::Operator& node) noexcept -> void {}

auto CVisitor::visit(ast::Expression& node) noexcept -> void {}

auto CVisitor::visit(ast::Binding& node) noexcept -> void {}

auto CVisitor::visit(ast::Print& node) noexcept -> void {}

auto CVisitor::visit(ast::Instruction& node) noexcept -> void {}

auto CVisitor::visit(ast::Program& node) noexcept -> void {}

auto CVisitor::visit(ast::Value& node) noexcept -> void {}

auto CVisitor::visit(ast::Constant& node) noexcept -> void {}

auto CVisitor::visit(ast::LocalVariable& node) noexcept -> void {}
