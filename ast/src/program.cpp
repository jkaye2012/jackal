#include "ast/program.hpp"

using jackal::ast::Instruction;
using jackal::ast::Program;

auto Program::add_instruction(Instruction instr) noexcept -> void
{
  _instructions.emplace_back(std::move(instr));
}
