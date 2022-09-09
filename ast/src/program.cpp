#include "ast/program.hpp"

#include <memory>

using jackal::ast::Instruction;
using jackal::ast::Program;

struct Instruction::Impl
{
  std::variant<Binding, Print> instruction;
};

Instruction::Instruction(Binding binding) noexcept
    : _impl(std::make_unique<Impl>(Impl{std::move(binding)}))
{
}

Instruction::Instruction(Print print) noexcept
    : _impl(std::make_unique<Impl>(Impl{std::move(print)}))
{
}

Instruction::~Instruction() = default;
Instruction::Instruction(Instruction&&) noexcept = default;
Instruction& Instruction::operator=(Instruction&&) noexcept = default;

auto Instruction::binding_unsafe() const noexcept -> Binding const&
{
  return std::get<Binding>(_impl->instruction);
}

auto Instruction::print_unsafe() const noexcept -> Print const&
{
  return std::get<Print>(_impl->instruction);
}

auto Instruction::instruction() noexcept -> std::variant<Binding, Print>&
{
  return _impl->instruction;
}
auto Instruction::instruction() const noexcept -> std::variant<Binding, Print> const&
{
  return _impl->instruction;
}

struct Program::Impl
{
  std::vector<Instruction> instructions;
};

Program::Program() noexcept : _impl(std::make_unique<Impl>()) {}

Program::~Program() = default;
Program::Program(Program&&) noexcept = default;
Program& Program::operator=(Program&&) noexcept = default;

auto Program::add_instruction(Instruction instr) noexcept -> void
{
  _impl->instructions.emplace_back(std::move(instr));
}

auto Program::instructions() noexcept -> std::vector<Instruction>& { return _impl->instructions; }

auto Program::instructions() const noexcept -> std::vector<Instruction> const&
{
  return _impl->instructions;
}
