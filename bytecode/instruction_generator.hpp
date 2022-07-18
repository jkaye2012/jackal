#pragma once

#include "bytecode/instruction.hpp"
#include "bytecode/opcode.hpp"
#include "bytecode/operand.hpp"
#include "bytecode/reg.hpp"

namespace jackal::bytecode::instructions
{
template <opcode Op>
struct unary_instruction : public instruction<1, true>
{
  constexpr unary_instruction(operand a, reg output) noexcept
      : instruction(Op, {a}, std::move(output))
  {
  }
};

struct instr_const : public unary_instruction<opcode::constant>
{
  using unary_instruction::unary_instruction;
};

struct instr_store : public unary_instruction<opcode::store>
{
  using unary_instruction::unary_instruction;
};

struct instr_load : public unary_instruction<opcode::load>
{
  using unary_instruction::unary_instruction;
};

template <opcode Op>
struct unary_void_instruction : public instruction<1, false>
{
  constexpr unary_void_instruction(operand a) noexcept : instruction(Op, {a}) {}
};

struct instr_print : public unary_void_instruction<opcode::print>
{
  using unary_void_instruction::unary_void_instruction;
};

template <opcode Op>
struct binary_instruction : public instruction<2, true>
{
  constexpr binary_instruction(operand a, operand b, reg output) noexcept
      : instruction(Op, {a, b}, std::move(output))
  {
  }
};

struct instr_add : public binary_instruction<opcode::add>
{
  using binary_instruction::binary_instruction;
};
}  // namespace jackal::bytecode::instructions
